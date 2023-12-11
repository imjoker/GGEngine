
#include <Engine/Math/Functions.h>

#include "Camera.h"

eae6320::Assets::Camera::Camera() noexcept
{
	rigidBody.position.x = 100.0f;
	rigidBody.position.y = 100.0f;
	rigidBody.position.z = 140.0f;
}

eae6320::Assets::Camera::~Camera() noexcept
{

}

void eae6320::Assets::Camera::Update(float i_secondCountToIntegrate) noexcept
{
	rigidBody.Update(i_secondCountToIntegrate);
}

void eae6320::Assets::Camera::AddVelocity(Math::sVector pVelocity) noexcept
{
	rigidBody.velocity += pVelocity;
}

eae6320::Math::cMatrix_transformation eae6320::Assets::Camera::CreateWorldToCameraTransform(float i_secondCountToExtrapolate) noexcept
{
	return eae6320::Math::cMatrix_transformation::CreateWorldToCameraTransform(rigidBody.PredictFutureTransform(i_secondCountToExtrapolate));
}

eae6320::Math::cMatrix_transformation eae6320::Assets::Camera::CreateCameraToProjectedTransform_perspective() noexcept
{
	return eae6320::Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(Math::ConvertHorizontalFieldOfViewToVerticalFieldOfView(eae6320::Math::ConvertDegreesToRadians(80), 1.0f), 1.0f, 0.1f, 100.0f);
}
