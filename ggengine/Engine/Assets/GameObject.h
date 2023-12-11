#pragma once

#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/CZPhysics/CZPhysics.h>
#include <Engine/CZPhysics/BoxCollider.h>

namespace eae6320 {

	namespace Assets {

		class GameObject {

		public:

											GameObject						(Graphics::VertexMesh* pMesh, Graphics::cEffect* pEffect, Math::sVector pPosition, Math::sVector pColliderExtents);
											~GameObject						();

			void							SetMesh							(Graphics::VertexMesh* pMesh) noexcept;
			void							SetEffect						(Graphics::cEffect* pEffect) noexcept;

			void							Update							(float i_secondCountToIntegrate) noexcept;

			void							Rotate							(const Math::cQuaternion& rotation) noexcept;

			void							SetVelocity						(Math::sVector pVelocity) noexcept;
			void							AddVelocity						(Math::sVector pVelocity) noexcept;

			void							SetVelocityToReachTarget		(const Math::sVector & target) noexcept;

			Math::sVector					GetPosition						() const noexcept;

			// Check if the object has reached the target
			bool							HasReachedTarget				(const Math::sVector& target) const;

			Math::cMatrix_transformation	GetTransformLocalToWorld		(float i_secondCountToExtrapolate) const noexcept;

			Graphics::VertexMesh*			GetMesh							() const noexcept;
			Graphics::cEffect*				GetEffect						() const noexcept;

			virtual void					OnCollisionEnter				(ChrisZ::Physics::Collider* other) {}
			virtual void					OnCollisionExit					(ChrisZ::Physics::Collider* other) {}
			virtual void					OnCollisionStay					(ChrisZ::Physics::Collider* other) {}

			void							SetPosition						(const Math::sVector& i_position);
			Math::cQuaternion				GetOrientation					() const;
			void							SetOrientation					(const Math::cQuaternion& i_orientation);

			inline ChrisZ::Physics::RigidBody* GetRigidBody					()	{ return m_pRigidBody; }
			inline ChrisZ::Physics::Collider* GetCollider					()	{ return m_pCollider;}

		private:

			Graphics::VertexMesh* m_mesh;
			Graphics::cEffect* m_effect;
			Math::sVector m_position;	// In arbitrary units determined by the applicaton's convention
			Math::cQuaternion m_orientation;
			ChrisZ::Physics::RigidBody* m_pRigidBody;
			ChrisZ::Physics::Collider* m_pCollider;

		};
	}
}