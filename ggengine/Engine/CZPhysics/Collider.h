#pragma once

#ifndef CZPHYSICS_COLLIDER_H
#define CZPHYSICS_COLLIDER_H

// Includes
//=========

#include <Engine/Math/cQuaternion.h>
#include <Engine/Math/sVector.h>

#include <unordered_set>

#endif // !CZPHYSICS_COLLIDER_H

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Math
	{
		class cMatrix_transformation;
	}

	namespace Assets
	{
		class GameObject;
	}
}

namespace ChrisZ
{
	namespace Physics
	{
		struct CollisionInfo;
	}
}

namespace ChrisZ
{
	namespace Physics
	{
		// A base class for colliders
		class Collider
		{
		public:
			// Constructor
			Collider(eae6320::Math::sVector i_centerOffset, eae6320::Assets::GameObject* i_gameObject);

			// Destructor
			~Collider();

			// A virtual method to check if this collider intersects with another collider
			virtual CollisionInfo Intersects(Collider* other) = 0;

			// A method to get the game object this collider is attached to
			eae6320::Assets::GameObject* GetGameObject();

			// A method to check if this collider can overlap with other colliders
			inline bool IsTrigger() const;

			// A method to set whether this collider can overlap with other colliders
			inline void SetIsTrigger(bool isTrigger);

			// A method to get the center of the collider
			inline eae6320::Math::sVector GetCenter() const;

			// A method to set the center of the collider
			inline void SetCenter(eae6320::Math::sVector center);

			// A method to get the offset of the center of the collider from the center of the game object
			inline eae6320::Math::sVector GetCenterOffset() const;

			// A method to set the offset of the center of the collider from the center of the game object
			inline void SetCenterOffset(eae6320::Math::sVector centerOffset);

			// A method to get the energy consumed after collision
			inline float GetRestitution() const;

			// A method to set the energy consumed after collision
			inline void SetRestitution(float restitution);

			// A method to get the friction coefficient
			inline float GetFrictionCoefficient() const;

			// A method to set the friction coefficient
			inline void SetFrictionCoefficient(float frictionCoefficient);

			// Method to check if a collider is colliding with self
			inline bool IsCollidingWith(Collider* other) const;

			// Method to add a collider to the hash set of colliding colliders
			inline void AddCollidingCollider(Collider* other);

			// Method to remove a collider from the hash set of colliding colliders
			inline void RemoveCollidingCollider(Collider* other);

		protected:
			// The center of the collider in world space
			eae6320::Math::sVector center;

			// The offset of the center of the collider from the center of the game object
			eae6320::Math::sVector centerOffset;

			// The energy consumed after collision
			float restitution = 0.3f;

			// The friction coefficient
			float frictionCoefficient = 0.5f;

			// Hash set of pointers to colliders that are colliding with self
			std::unordered_set<Collider*> collidingColliders;

			// Whether this collider can overlap with other colliders
			bool isTrigger = false;

			// The game object that this collider is attached to
			eae6320::Assets::GameObject* gameObject;
		};
	}
}

#include "Collider.inl"