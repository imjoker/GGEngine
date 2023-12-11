#include "Collider.h"

#include <Engine/CZPhysics/CZPhysics.h>
#include <Engine/Assets/GameObject.h>

ChrisZ::Physics::Collider::Collider(eae6320::Math::sVector i_centerOffset, eae6320::Assets::GameObject* i_gameObject) : centerOffset(i_centerOffset), gameObject(i_gameObject)
{
	center = i_gameObject->GetPosition() + i_centerOffset;

	// Add this collider to the physics system
	ChrisZ::Physics::AddCollider(this);
}

ChrisZ::Physics::Collider::~Collider()
{
	// Remove this collider from the physics system
	ChrisZ::Physics::RemoveCollider(this);

	// Remove this collider from all colliding colliders
	for (auto it = collidingColliders.begin(); it != collidingColliders.end(); ++it)
	{
		(*it)->RemoveCollidingCollider(this);
	}

	// Set the game object to null
	this->gameObject = nullptr;
}

eae6320::Assets::GameObject* ChrisZ::Physics::Collider::GetGameObject()
{
	return this->gameObject;
}