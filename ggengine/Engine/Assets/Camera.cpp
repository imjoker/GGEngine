
#include <Engine/Math/Functions.h>

#include "Camera.h"

eae6320::Assets::Camera::Camera() noexcept
{
	rigidBody.position.x = 0.0f;
	rigidBody.position.y = 0.0f;
	rigidBody.position.z = 10.0f;
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

	//cResult Camera::Load(Camera*& o_Camera)
	//{
	//	auto result = Results::Success;

	//	Camera* newCamera = nullptr;
	//	cScopeGuard scopeGuard([&o_Camera, &result, &newCamera]
	//		{
	//			if (result)
	//			{
	//				EAE6320_ASSERT(newMesh != nullptr);
	//				o_Camera = newCamera;
	//			}
	//			else
	//			{
	//				if (newCamera)
	//				{
	//					newCamera->DecrementReferenceCount();
	//					newCamera = nullptr;
	//				}
	//				o_Camera = nullptr;
	//			}
	//		});

	//	// Allocate a camera
	//	{
	//		newCamera = new (std::nothrow) Camera();
	//		if (!newCamera)
	//		{
	//			result = Results::OutOfMemory;
	//			EAE6320_ASSERTF(false, "Couldn't allocate memory for the camera");
	//			Logging::OutputError("Failed to allocate memory for the camera");
	//			return result;
	//		}
	//	}

	//	return result;
	//}
