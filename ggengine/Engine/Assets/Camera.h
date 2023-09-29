#pragma once

#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Physics/sRigidBodyState.h>

namespace eae6320 {

	namespace Assets {

		class Camera {

		public:

											Camera										() noexcept;
											~Camera										() noexcept;

			void							Update										(float i_secondCountToIntegrate) noexcept;

			void							AddVelocity									(Math::sVector pVelocity) noexcept;

			Math::cMatrix_transformation	CreateWorldToCameraTransform				(float i_secondCountToExtrapolate) noexcept;
			Math::cMatrix_transformation	CreateCameraToProjectedTransform_perspective() noexcept;

		private:
			Physics::sRigidBodyState rigidBody{};
		};
	}
}
