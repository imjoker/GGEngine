#pragma once

#include <Engine/CZPhysics/Collider.h>

namespace ChrisZ
{
	namespace Physics
	{
		class BoxCollider : public Collider
		{
		public:
			// A constructor that takes the center, size, orientation and game object as parameters
			BoxCollider(eae6320::Math::sVector i_centerOffset, eae6320::Math::sVector i_extents, eae6320::Assets::GameObject* i_gameObject);

			// A method to check if this box collider intersects with another collider
			CollisionInfo Intersects(Collider* other) override;

			// A method to get the extents of the box
			inline eae6320::Math::sVector GetExtents() const;

			// A method to set the extents of the box
			inline void SetExtents(eae6320::Math::sVector i_extents);

			// Helper Methods
			//===============

			// A method to get the closest point on the box to a given point
			eae6320::Math::sVector ClosestPoint(eae6320::Math::sVector point);

			// Calculate the vertices of the box
			void CalculateVertices();

			// Define a helper method to project the box onto an axis
			void ProjectOntoAxis(eae6320::Math::sVector axis, float& min, float& max);

			// Use the bounding sphere detection as a broad phase detection to check if this box collider has a possibility of intersecting with another box collider
			// Complexity: O(1)
			inline bool BoundingSphereDetection(BoxCollider* other);

			// Use the AABB method as a middle phase detection to check if this box collider has a possibility of intersecting with another box collider
			// Complexity: O(n)
			bool AABBDetection(BoxCollider* other);

			// Use the AABB method as the final phase detection if the boxes are axis aligned
			CollisionInfo AABBDetectionAxisAligned(BoxCollider* other);

			// Use the SAT method to check if this box collider intersects with another box collider
			// Complexity: O(n ^ 2)
			CollisionInfo SATDetection(BoxCollider* other);

			inline bool isAxisAligned(eae6320::Math::cQuaternion orientation);

		private:
			eae6320::Math::sVector extents = eae6320::Math::sVector(0.5f, 0.5f, 0.5f); // The half-size of the box along each axis
			eae6320::Math::sVector vertices[8]; // The vertices of the box
		};
	}
}

#include "BoxCollider.inl"