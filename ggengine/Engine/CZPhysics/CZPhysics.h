#pragma once
/*
	This file declares the external interface for the physics system
*/

#ifndef CZPHYSICS_H
#define CZPHYSICS_H

// Includes
//=========

#include "RigidBody.h"
#include "Collider.h"

#endif // !CZPHYSICS_H

namespace ChrisZ
{
	namespace Physics
	{
		// Struct to store collision information
		struct CollisionInfo {
			// Contact normal of the collision
			eae6320::Math::sVector contactNormal;

			// Penetration depth of the collision
			float penetrationDepth;

			CollisionInfo(const eae6320::Math::sVector& contactNormal, float penetrationDepth)
				: contactNormal(contactNormal), penetrationDepth(penetrationDepth)
			{
			}
		};

		void AddRigidBody(RigidBody* i_rigidBody);
		void AddCollider(Collider* i_collider);
		void RemoveRigidBody(RigidBody* i_rigidBody);
		void RemoveCollider(Collider* i_collider);

		void Update(const float i_secondCountToIntegrate);

		// Helper functions
		void HandleCollision(Collider* collider, Collider* other, CollisionInfo collisionInfo, const float i_secondCountToIntegrate);
	}
}