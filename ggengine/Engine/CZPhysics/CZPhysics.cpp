#include "CZPhysics.h"

#include <Engine/Assets/GameObject.h>
#include <Engine/CZPhysics/SphereCollider.h>
#include <Engine/CZPhysics/BoxCollider.h>

#include <vector>
#include <algorithm>

#include <Engine/Logging/Logging.h>

#define VELOCITY_THRESHOLD 0.01f

// A list of all rigid bodies
std::vector<ChrisZ::Physics::RigidBody*> rigidBodies;
// A list of all colliders
std::vector<ChrisZ::Physics::Collider*> colliders;

void ChrisZ::Physics::AddRigidBody(RigidBody* i_rigidBody)
{
    rigidBodies.push_back(i_rigidBody);
}

void ChrisZ::Physics::AddCollider(Collider* i_collider)
{
    colliders.push_back(i_collider);
}

void ChrisZ::Physics::RemoveRigidBody(RigidBody* i_rigidBody)
{
    //rigidBodies.erase(std::remove(rigidBodies.begin(), rigidBodies.end(), i_rigidBody), rigidBodies.end());
}

void ChrisZ::Physics::RemoveCollider(Collider* i_collider)
{
	//colliders.erase(std::remove(colliders.begin(), colliders.end(), i_collider), colliders.end());
}

void ChrisZ::Physics::Update(const float i_secondCountToIntegrate)
{
    // Update rigid bodies
    {
        for (auto rigidBody : rigidBodies)
        {
            rigidBody->Update(i_secondCountToIntegrate);
        }
    }
	// Check for collisions
	{
		// Iterate the colliders with an iterator
        for (auto i = colliders.begin(); i != colliders.end(); ++i)
        {
            for (auto j = std::next(i); j != colliders.end(); ++j)
            {
				Collider* collider = *i;
				Collider* other = *j;

                if (collider != other && collider && other)
                {
                    // Call Intersects to check for collision and get CollisionInfo
                    CollisionInfo collisionInfo = collider->Intersects(other);

                    // Check if there's a collision based on penetration depth
                    if (collisionInfo.penetrationDepth > 0.0f)
                    {
                        // Add colliding colliders to each other's lists if not already added
                        if (!collider->IsCollidingWith(other))
                        {
                            collider->AddCollidingCollider(other);
                            other->AddCollidingCollider(collider);

                            // Notify GameObjects about the collision
                            collider->GetGameObject()->OnCollisionEnter(other);
							other->GetGameObject()->OnCollisionEnter(collider);
						}

						bool isValid1 = collider->GetGameObject() != nullptr;
						bool isValid2 = other->GetGameObject() != nullptr;
						// Notify GameObjects about the collision
						collider->GetGameObject()->OnCollisionStay(other);
						other->GetGameObject()->OnCollisionStay(collider);

						// Handle collision if the two colliders are not triggers
						if (!(collider->IsTrigger() || other->IsTrigger()))
						{
							HandleCollision(collider, other, collisionInfo, i_secondCountToIntegrate);
						}
                    }
                    else
                    {
                        // If no collision, remove colliders from each other's lists if they are present
                        if (collider->IsCollidingWith(other))
                        {
                            collider->RemoveCollidingCollider(other);
                            other->RemoveCollidingCollider(collider);

                            // Notify GameObjects about the collision
                            collider->GetGameObject()->OnCollisionExit(other);
                            other->GetGameObject()->OnCollisionExit(collider);
                        }
                    }
                }
			}
		}
	}
}

void ChrisZ::Physics::HandleCollision(Collider* collider, Collider* other, CollisionInfo collisionInfo, const float i_secondCountToIntegrate)
{
    // Get the rigid bodies of the colliding colliders
    RigidBody* bodyA = collider->GetGameObject()->GetRigidBody();
    RigidBody* bodyB = other->GetGameObject()->GetRigidBody();

    // Relative velocity and masses
    eae6320::Math::sVector velocityA = (bodyA) ? bodyA->GetVelocity() : eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
    eae6320::Math::sVector angularVelocityA = (bodyA) ? bodyA->GetAngularVelocity() : eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
    float massA = (bodyA) ? bodyA->GetMass() : FLT_MAX;

    eae6320::Math::sVector velocityB = (bodyB) ? bodyB->GetVelocity() : eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
    eae6320::Math::sVector angularVelocityB = (bodyB) ? bodyB->GetAngularVelocity() : eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
    float massB = (bodyB) ? bodyB->GetMass() : FLT_MAX;

    // Calculate the relative velocity of the two bodies along the collision normal
    eae6320::Math::sVector relativeVelocity = velocityB - velocityA;

    // Calculate the relative velocity of the two bodies along the collision normal
    float velocityAlongNormal = Dot(relativeVelocity, collisionInfo.contactNormal);

    // Do nothing if the bodies are moving away from each other
    if (velocityAlongNormal > 0.0f)
    {
        return;
    }

    // Calculate the restitution coefficient based on the bounciness of the two bodies
    float e = std::min(collider->GetRestitution(), other->GetRestitution());

    // Calculate the impulse scalar
    float j = -(1 + e) * velocityAlongNormal;
    j /= 1 / massA + 1 / massB;

    // Apply the impulse force to the bodies
    eae6320::Math::sVector impulse = j * collisionInfo.contactNormal;

    // Apply the impulse torque to the bodies (angular velocity)
    eae6320::Math::sVector torqueA = eae6320::Math::Cross(collisionInfo.contactNormal, impulse);
    eae6320::Math::sVector torqueB = eae6320::Math::Cross(collisionInfo.contactNormal, -impulse);

    // Check if bodyA has a rigid body
    if (bodyA)
    {
        bodyA->AddImpulse(-impulse);
        bodyA->AddTorque(torqueA);
    }
    // Check if bodyB has a rigid body
    if (bodyB)
    {
        bodyB->AddImpulse(impulse);
        bodyB->AddTorque(torqueB);
    }

    // Calculate the amount of position correction needed to prevent interpenetration
    // A small percentage of the penetration depth is used to avoid jittering
    const float percent = 0.2f;
    // A small value is added to the penetration depth to avoid division by zero
    const float slop = 0.01f;
    eae6320::Math::sVector correction = (std::max(collisionInfo.penetrationDepth - slop, 0.0f) / (1 / massA + 1 / massB)) * percent * collisionInfo.contactNormal;

    // Correct the positions of the bodies
    // Check if bodyA has a rigid body
    if (bodyA)
    {
        bodyA->GetGameObject()->SetPosition(bodyA->GetGameObject()->GetPosition() - (1 / massA) * correction);
    }
    // Check if bodyB has a rigid body
    if (bodyB)
    {
        bodyB->GetGameObject()->SetPosition(bodyB->GetGameObject()->GetPosition() + (1 / massB) * correction);
    }


    // Ignore velocities below the threshold
    if (abs(relativeVelocity.x) < VELOCITY_THRESHOLD)
    {
        relativeVelocity.x = 0.0f;
    }
    if (abs(relativeVelocity.y) < VELOCITY_THRESHOLD)
	{
		relativeVelocity.y = 0.0f;
	}
    if (abs(relativeVelocity.z) < VELOCITY_THRESHOLD)
	{
		relativeVelocity.z = 0.0f;
	}

	// Calculate the coefficient of friction between the two bodies
	float mu = collider->GetFrictionCoefficient() * other->GetFrictionCoefficient();

	// If the relative velocity is zero or it is parallel to the collision normal, or the friction is zero, do nothing
	if (relativeVelocity == eae6320::Math::sVector(0.0f, 0.0f, 0.0f) || Cross(relativeVelocity, collisionInfo.contactNormal) == eae6320::Math::sVector(0.0f, 0.0f, 0.0f) || mu == 0.0f)
	{
		return;
	}

	// Calculate the tangent vector of the collision
	eae6320::Math::sVector tangent = relativeVelocity - (Dot(relativeVelocity, collisionInfo.contactNormal) * collisionInfo.contactNormal);
	tangent.Normalize();

	// Calculate the magnitude of the friction impulse
	float jt = -Dot(relativeVelocity, tangent);
	jt /= 1 / massA + 1 / massB;

	// Clamp the friction impulse to avoid reversing the direction of motion
	if (abs(jt) > j * mu)
	{
		jt = j * mu * (jt < 0 ? -1.0f : 1.0f);
	}

	// Apply the friction impulse to the bodies
	eae6320::Math::sVector frictionImpulse = jt * tangent;
	// Check if bodyA has a rigid body
	if (bodyA)
	{
		bodyA->AddImpulse(-frictionImpulse);
	}
	// Check if bodyB has a rigid body
	if (bodyB)
	{
		bodyB->AddImpulse(frictionImpulse);
	}

    // Apply the friction torque to the bodies (angular velocity)
    eae6320::Math::sVector frictionTorqueA = eae6320::Math::Cross(collisionInfo.contactNormal, frictionImpulse);
    eae6320::Math::sVector frictionTorqueB = eae6320::Math::Cross(collisionInfo.contactNormal, -frictionImpulse);

    // Check if bodyA has a rigid body
    if (bodyA)
    {
        bodyA->AddTorque(frictionTorqueA);
    }
    // Check if bodyB has a rigid body
    if (bodyB)
    {
        bodyB->AddTorque(frictionTorqueB);
    }
}