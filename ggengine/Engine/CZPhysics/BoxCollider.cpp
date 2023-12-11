#include "BoxCollider.h"
#include "SphereCollider.h"

#include <Engine/CZPhysics/CZPhysics.h>

#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Assets/GameObject.h>

#include <cfloat>
#include <algorithm>

#define ZERO_VECTOR_THRESHOLD 0.1f

ChrisZ::Physics::BoxCollider::BoxCollider(eae6320::Math::sVector i_centerOffset, eae6320::Math::sVector i_extents, eae6320::Assets::GameObject* i_gameObject) : Collider(i_centerOffset, i_gameObject), extents(i_extents)
{
}

ChrisZ::Physics::CollisionInfo ChrisZ::Physics::BoxCollider::Intersects(Collider* other)
{
    // If the other collider is a sphere collider, use the sphere-box intersection formula
    if (SphereCollider* otherSphere = dynamic_cast<SphereCollider*>(other))
    {
        // Get the closest point on the box to the sphere center
        eae6320::Math::sVector closestPoint = this->ClosestPoint(otherSphere->GetCenter());

        // Get the distance between the closest point and the sphere center
        float distance = (otherSphere->GetCenter() - closestPoint).GetLength();

        // If the distance is less than or equal to the radius, the sphere and the box intersect
        if (distance <= otherSphere->GetRadius())
        {
            // Calculate and return CollisionInfo
            eae6320::Math::sVector contactNormal = (otherSphere->GetCenter() - closestPoint).GetNormalized();
            float penetrationDepth = otherSphere->GetRadius() - distance;
            return CollisionInfo(contactNormal, penetrationDepth);
        }
        else
        {
			// No collision
			return CollisionInfo(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);
		}
    }

    // If the other collider is a box collider, use the box-box intersection detections
    if (BoxCollider* otherBox = dynamic_cast<BoxCollider*>(other))
    {
        // Use the bounding sphere detection as a broad phase detection to check if the boxes have a possibility of intersecting
        if (!this->BoundingSphereDetection(otherBox))
        {
            // No collision
            return CollisionInfo(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);
        }

        // If the boxes are axis aligned, use the AABB method as the final phase detection
        if (isAxisAligned(this->gameObject->GetOrientation()) && isAxisAligned(otherBox->GetGameObject()->GetOrientation()))
        {
			// Use the AABB method as the final phase detection
			return this->AABBDetectionAxisAligned(otherBox);
		}
        // If the boxes are not axis aligned, use the AABB method as a middle phase detection
		else 
        {
            // Calculate the vertices of the boxes
            CalculateVertices();
            otherBox->CalculateVertices();

            if (!this->AABBDetection(otherBox))
            {
				// No collision
				return CollisionInfo(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);
			}
        }

        // Use the SAT method to check if the boxes intersect and return the CollisionInfo
        return this->SATDetection(otherBox);
    }

    // No collision
    return CollisionInfo(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);
}

// Helper Methods
//===============

eae6320::Math::sVector ChrisZ::Physics::BoxCollider::ClosestPoint(eae6320::Math::sVector point)
{
    // Get the orientation of the box
    eae6320::Math::cQuaternion orientation = gameObject->GetOrientation();

    // Transform the point to the local space of the box
    eae6320::Math::sVector localPoint = orientation.GetInverse() * (point - center);

    // Clamp the local coordinates of the point to the box extents
    eae6320::Math::sVector localClosest = localPoint;
    localClosest.x = std::max(-extents.x, std::min(extents.x, localPoint.x));
    localClosest.y = std::max(-extents.y, std::min(extents.y, localPoint.y));
    localClosest.z = std::max(-extents.z, std::min(extents.z, localPoint.z));

    // Transform the closest point back to world space
    eae6320::Math::sVector worldClosest = center + (orientation * localClosest);

    return worldClosest;
}

void ChrisZ::Physics::BoxCollider::CalculateVertices()
{
    // Get the orientation matrix of the box
    eae6320::Math::cMatrix_transformation orientation = eae6320::Math::cMatrix_transformation(gameObject->GetOrientation(), center);

    // Get the right, up, and back vectors of the box
    eae6320::Math::sVector right = orientation.GetRightDirection() * extents.x;
    eae6320::Math::sVector up = orientation.GetUpDirection() * extents.y;
    eae6320::Math::sVector back = orientation.GetBackDirection() * extents.z;

    // Calculate the eight vertices of the box by adding or subtracting the right, up, and back vectors from the center
    this->vertices[0] = this->center + right + up + back; // front top right
    this->vertices[1] = this->center + right + up - back; // front top left
    this->vertices[2] = this->center + right - up + back; // front bottom right
    this->vertices[3] = this->center + right - up - back; // front bottom left
    this->vertices[4] = this->center - right + up + back; // back top right
    this->vertices[5] = this->center - right + up - back; // back top left
    this->vertices[6] = this->center - right - up + back; // back bottom right
    this->vertices[7] = this->center - right - up - back; // back bottom left
}

void ChrisZ::Physics::BoxCollider::ProjectOntoAxis(eae6320::Math::sVector axis, float& min, float& max)
{
    // Initialize the min and max values to the first vertex projection
    min = max = eae6320::Math::Dot(this->vertices[0], axis);

    // Loop through the remaining vertices and update the min and max values
    for (int i = 1; i < 8; i++)
    {
        // Project the vertex onto the axis
        float projection = eae6320::Math::Dot(this->vertices[i], axis);

        // Update the min and max values
        if (projection < min)
        {
            min = projection;
        }
        else if (projection > max)
        {
            max = projection;
        }
    }
}

bool ChrisZ::Physics::BoxCollider::AABBDetection(BoxCollider* other)
{
    // Find the min and max points for this box
    eae6320::Math::sVector minThis = this->vertices[0];
    eae6320::Math::sVector maxThis = this->vertices[0];

    for (int i = 1; i < 8; i++)
	{
		// Update the min and max points
		if (this->vertices[i].x < minThis.x)
		{
			minThis.x = this->vertices[i].x;
		}
		else if (this->vertices[i].x > maxThis.x)
		{
			maxThis.x = this->vertices[i].x;
		}

		if (this->vertices[i].y < minThis.y)
		{
			minThis.y = this->vertices[i].y;
		}
		else if (this->vertices[i].y > maxThis.y)
		{
			maxThis.y = this->vertices[i].y;
		}

		if (this->vertices[i].z < minThis.z)
		{
			minThis.z = this->vertices[i].z;
		}
		else if (this->vertices[i].z > maxThis.z)
		{
			maxThis.z = this->vertices[i].z;
		}
	}

	// Find the min and max points for the other box
	eae6320::Math::sVector minOther = other->vertices[0];
	eae6320::Math::sVector maxOther = other->vertices[0];

    for (int i = 1; i < 8; i++)
    {
        // Update the min and max points
		if (other->vertices[i].x < minOther.x)
		{
			minOther.x = other->vertices[i].x;
		}
		else if (other->vertices[i].x > maxOther.x)
		{
			maxOther.x = other->vertices[i].x;
		}

		if (other->vertices[i].y < minOther.y)
		{
			minOther.y = other->vertices[i].y;
		}
		else if (other->vertices[i].y > maxOther.y)
		{
			maxOther.y = other->vertices[i].y;
		}

		if (other->vertices[i].z < minOther.z)
		{
			minOther.z = other->vertices[i].z;
		}
		else if (other->vertices[i].z > maxOther.z)
		{
			maxOther.z = other->vertices[i].z;
		}
	}

    // Check for overlap along the x-axis
    bool overlapX = (maxThis.x > minOther.x) && (minThis.x < maxOther.x);

    // Check for overlap along the y-axis
    bool overlapY = (maxThis.y > minOther.y) && (minThis.y < maxOther.y);

    // Check for overlap along the z-axis
    bool overlapZ = (maxThis.z > minOther.z) && (minThis.z < maxOther.z);

    // Return true if there is overlap along all axes
    return overlapX && overlapY && overlapZ;
}

ChrisZ::Physics::CollisionInfo ChrisZ::Physics::BoxCollider::AABBDetectionAxisAligned(BoxCollider* other)
{
    // A common AABB method for axis aligned boxes
    eae6320::Math::sVector minA = this->center - this->extents;
    eae6320::Math::sVector maxA = this->center + this->extents;
    eae6320::Math::sVector minB = other->GetCenter() - other->extents;
    eae6320::Math::sVector maxB = other->GetCenter() + other->extents;

    // Check if the min and max points overlap along each axis
    if (maxA.x > minB.x && minA.x < maxB.x && maxA.y > minB.y && minA.y < maxB.y && maxA.z > minB.z && minA.z < maxB.z)
    {
		// Calculate CollisionInfo

        eae6320::Math::sVector contactNormal;

        // The contact normal is the axis of least penetration
        float penetrationDepth = FLT_MAX;
        if (maxA.x - minB.x < penetrationDepth)
		{
			contactNormal = eae6320::Math::sVector(1.0f, 0.0f, 0.0f);
			penetrationDepth = maxA.x - minB.x;
		}
        if (maxB.x - minA.x < penetrationDepth)
		{
			contactNormal = eae6320::Math::sVector(-1.0f, 0.0f, 0.0f);
			penetrationDepth = maxB.x - minA.x;
		}

		if (maxA.y - minB.y < penetrationDepth)
		{
			contactNormal = eae6320::Math::sVector(0.0f, 1.0f, 0.0f);
			penetrationDepth = maxA.y - minB.y;
		}

		if (maxB.y - minA.y < penetrationDepth)
		{
			contactNormal = eae6320::Math::sVector(0.0f, -1.0f, 0.0f);
			penetrationDepth = maxB.y - minA.y;
		}

		if (maxA.z - minB.z < penetrationDepth)
		{
			contactNormal = eae6320::Math::sVector(0.0f, 0.0f, 1.0f);
			penetrationDepth = maxA.z - minB.z;
		}

		if (maxB.z - minA.z < penetrationDepth)
		{
            contactNormal = eae6320::Math::sVector(0.0f, 0.0f, -1.0f);
			penetrationDepth = maxB.z - minA.z;
		}

		return CollisionInfo(contactNormal, penetrationDepth);
	}
    else
    {
		// No collision
		return CollisionInfo(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);
	}
}

ChrisZ::Physics::CollisionInfo ChrisZ::Physics::BoxCollider::SATDetection(BoxCollider* otherBox)
{
    // Get the axes of the boxes
    eae6320::Math::sVector axes[15];

    // Get the forward vector of this box
    axes[2] = this->gameObject->GetOrientation().CalculateForwardDirection();
    // Get the right vector of this box
    axes[0] = this->gameObject->GetOrientation() * eae6320::Math::Cross(axes[2], eae6320::Math::sVector(0.0f, 1.0f, 0.0f));
    // Get the up vector of this box by crossing the right vector with the forward vector
    axes[1] = eae6320::Math::Cross(axes[0], axes[2]);
    // Get the forward vector of the other box
    axes[5] = otherBox->GetGameObject()->GetOrientation().CalculateForwardDirection();
    // Get the right vector of the other box by crossing the forward vector with the world up vector
    axes[3] = otherBox->GetGameObject()->GetOrientation() * eae6320::Math::Cross(axes[5], eae6320::Math::sVector(0.0f, 1.0f, 0.0f));
    // Get the up vector of the other box by crossing the right vector with the forward vector
    axes[4] = eae6320::Math::Cross(axes[3], axes[5]);

    // Cross products between the axes to get additional axes
    axes[6] = eae6320::Math::Cross(axes[0], axes[3]);
    axes[7] = eae6320::Math::Cross(axes[0], axes[4]);
    axes[8] = eae6320::Math::Cross(axes[0], axes[5]);
    axes[9] = eae6320::Math::Cross(axes[1], axes[3]);
    axes[10] = eae6320::Math::Cross(axes[1], axes[4]);
    axes[11] = eae6320::Math::Cross(axes[1], axes[5]);
    axes[12] = eae6320::Math::Cross(axes[2], axes[3]);
    axes[13] = eae6320::Math::Cross(axes[2], axes[4]);
    axes[14] = eae6320::Math::Cross(axes[2], axes[5]);

    // Check if any of the axes are zero vectors
    for (int i = 0; i < 15; ++i)
	{
		if (abs(axes[i].x) < ZERO_VECTOR_THRESHOLD && abs(axes[i].y) < ZERO_VECTOR_THRESHOLD && abs(axes[i].z) < ZERO_VECTOR_THRESHOLD)
		{
            // Give the axis a default value
			axes[i] = eae6320::Math::sVector(1.0f, 0.0f, 0.0f);
		}
	}

    float penetrationDepth = FLT_MAX;
    eae6320::Math::sVector contactNormal;

    // Project the vertices of both boxes onto each axis and check for overlap
    for (int i = 0; i < 15; ++i)
    {
        float minThis, maxThis, minOther, maxOther;
        ProjectOntoAxis(axes[i], minThis, maxThis);
        otherBox->ProjectOntoAxis(axes[i], minOther, maxOther);

        // Check for overlap
        if (maxThis < minOther || minThis > maxOther)
        {
            // No overlap on this axis, so there is no collision
            return CollisionInfo(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);
        }
        
        // Calculate the smallest penetration depth and corresponding contact normal
        float overlap = std::min(maxThis, maxOther) - std::max(minThis, minOther);

        if (overlap < penetrationDepth)
        {
            penetrationDepth = overlap;
            contactNormal = axes[i];
        }
    }

    // Ensure the contact normal points from the current box to the other box
    eae6320::Math::sVector fromCurrentToOther = otherBox->GetCenter() - this->center;
    if (eae6320::Math::Dot(contactNormal, fromCurrentToOther) < 0)
    {
        contactNormal = -contactNormal;
    }

    return CollisionInfo(contactNormal.GetNormalized(), penetrationDepth);
}