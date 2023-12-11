#pragma once

#include <Engine/CZPhysics/Collider.h>

namespace ChrisZ
{
	namespace Physics
	{
		class SphereCollider : public Collider
		{
		public:
			// A constructor that takes the center, radius and game object as parameters
			SphereCollider(eae6320::Math::sVector i_centerOffset, float i_radius, eae6320::Assets::GameObject* i_gameObject);

			// A method to check if this sphere collider intersects with another collider
			CollisionInfo Intersects(Collider* other) override;

			inline float GetRadius() const;
			inline void SetRadius(float i_radius);

		private:
			float radius = 0.0f;
		};
	}
}

inline float ChrisZ::Physics::SphereCollider::GetRadius() const
{
	return this->radius;
}

inline void ChrisZ::Physics::SphereCollider::SetRadius(float i_radius)
{
	this->radius = i_radius;
}