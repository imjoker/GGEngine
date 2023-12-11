#pragma once

#include "BoxCollider.h"

inline eae6320::Math::sVector ChrisZ::Physics::BoxCollider::GetExtents() const
{
	return this->extents;
}

inline void ChrisZ::Physics::BoxCollider::SetExtents(const eae6320::Math::sVector i_extents)
{
	this->extents = i_extents;
}

inline bool ChrisZ::Physics::BoxCollider::BoundingSphereDetection(BoxCollider* other)
{
    // Get the distance between the centers of the two boxes
    eae6320::Math::sVector distance = center - other->GetCenter();

    // Get the sum of the radii of the two bounding spheres
    float sumRadii = extents.GetLength() + other->extents.GetLength();

    // Check if the distance is less than the sum of the radii
    if (distance.GetLength() < sumRadii)
    {
        // There is a possibility of intersection
        return true;
    }
    else
    {
        // There is no intersection
        return false;
    }
}

inline bool ChrisZ::Physics::BoxCollider::isAxisAligned(eae6320::Math::cQuaternion orientation)
{
    // Check if the orientation is axis aligned
    eae6320::Math::sVector exampleVector = orientation * eae6320::Math::sVector(1.0f, 1.0f, 1.0f);
    return (exampleVector.x == 1.0f && exampleVector.y == 1.0f && exampleVector.z == 1.0f);
}