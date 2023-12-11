#include "SphereCollider.h"
#include "BoxCollider.h"

#include <Engine/CZPhysics/CZPhysics.h>

#include <Engine/Assets/GameObject.h>

ChrisZ::Physics::SphereCollider::SphereCollider(eae6320::Math::sVector i_centerOffset, float i_radius, eae6320::Assets::GameObject* i_gameObject) : Collider(i_centerOffset, i_gameObject), radius(i_radius)
{
}

ChrisZ::Physics::CollisionInfo ChrisZ::Physics::SphereCollider::Intersects(Collider* other)
{
    // If the other collider is a sphere collider, use the sphere-sphere intersection formula
    if (SphereCollider* otherSphere = dynamic_cast<SphereCollider*>(other)) {
        // Get the distance between the centers of the spheres
        float distance = (this->center - otherSphere->center).GetLength();

        // If the distance is less than or equal to the sum of the radius, the spheres intersect
        if (distance <= (this->radius + otherSphere->radius)) {
            // Calculate and return CollisionInfo
            eae6320::Math::sVector contactNormal = (otherSphere->center - this->center).GetNormalized();
            float penetrationDepth = (this->radius + otherSphere->radius) - distance;
            return CollisionInfo(contactNormal, penetrationDepth);
        }
    }

    // If the other collider is a box collider, use the sphere-box intersection formula
    if (BoxCollider* otherBox = dynamic_cast<BoxCollider*>(other)) {
        // Get the closest point on the box to the sphere center
        eae6320::Math::sVector closestPoint = otherBox->ClosestPoint(this->center);

        // Get the distance between the closest point and the sphere center
        float distance = (this->center - closestPoint).GetLength();

        // If the distance is less than or equal to the radius, the sphere and the box intersect
        if (distance <= this->radius) {
            // Calculate and return CollisionInfo
            eae6320::Math::sVector contactNormal = (closestPoint - this->center).GetNormalized();
            float penetrationDepth = this->radius - distance;
            return CollisionInfo(contactNormal, penetrationDepth);
        }
    }

    // No collision
    return CollisionInfo(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);
}