#pragma once

#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Graphics/Graphics.h>

namespace eae6320 {

	namespace Assets {

		class GameObject {

		public:

											GameObject						(Graphics::VertexMesh* pMesh, Graphics::cEffect* pEffect, Physics::sRigidBodyState & pRigidBody);
											~GameObject						();

			void							SetMesh							(Graphics::VertexMesh* pMesh) noexcept;
			void							SetEffect						(Graphics::cEffect* pEffect) noexcept;

			void							Update							(float i_secondCountToIntegrate) noexcept;

			void							AddVelocity						(Math::sVector pVelocity) noexcept;

			Math::cMatrix_transformation	GetTransformLocalToWorld		(float i_secondCountToExtrapolate) const noexcept;

			Graphics::VertexMesh*			GetMesh							() const noexcept;
			Graphics::cEffect*				GetEffect						() const noexcept;



		private:

			Graphics::VertexMesh* m_mesh;
			Graphics::cEffect* m_effect;
			Physics::sRigidBodyState m_rigidBody;

		};
	}
}