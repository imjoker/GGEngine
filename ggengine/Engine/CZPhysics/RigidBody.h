#pragma once

#include <Engine/Math/cQuaternion.h>
#include <Engine/Math/sVector.h>
#include <Engine/Math/cMatrix_transformation.h>

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Assets
	{
		class GameObject;
	}
}

namespace ChrisZ
{
	namespace Physics
	{
		class RigidBody
		{
		public:
			// Constructor
			RigidBody(eae6320::Assets::GameObject* i_gameObject);

			// Destructor
			~RigidBody();

			// Interface
			//==========
			inline void AddForce(const eae6320::Math::sVector i_force);
			//void AddForceAtLocation(const eae6320::Math::sVector i_force, const eae6320::Math::sVector i_pointOfImpact);
			inline void AddImpulse(const eae6320::Math::sVector i_impulse);
			//void AddImpulseAtLocation(const eae6320::Math::sVector i_impulse, const eae6320::Math::sVector i_pointOfImpact);
			inline void AddTorque(const eae6320::Math::sVector i_torque);

			// Getters and Setters
			//====================

			eae6320::Assets::GameObject* GetGameObject() const;
			inline eae6320::Math::sVector GetVelocity() const;
			inline void SetVelocity(eae6320::Math::sVector i_velocity);
			inline eae6320::Math::sVector GetAcceleration() const;
			inline void SetAcceleration(eae6320::Math::sVector i_acceleration);
			inline eae6320::Math::sVector GetAngularVelocity() const;
			inline void SetAngularVelocity(eae6320::Math::sVector i_angularVelocity);
			inline bool GetRotationLocked(int i_axis) const;
			inline void SetRotationLocked(bool i_rotationLocked_x, bool i_rotationLocked_y, bool i_rotationLocked_z);
			inline float GetDragCoefficient() const;
			inline void SetDragCoefficient(float i_dragCoefficient);
			inline float GetMass() const;
			inline void SetMass(float i_mass);
			inline bool GetGravityEnabled() const;
			inline void SetGravityEnabled(bool i_gravityEnabled);

			// Interface
			//==========

			void Update(const float i_secondCountToIntegrate);
			eae6320::Math::sVector PredictFuturePosition(const float i_secondCountToExtrapolate) const;
			eae6320::Math::cQuaternion PredictFutureOrientation(const float i_secondCountToExtrapolate) const;
			inline eae6320::Math::cMatrix_transformation PredictFutureTransform(const float i_secondCountToExtrapolate) const;

		private:
			eae6320::Assets::GameObject* gameObject = nullptr;

			eae6320::Math::sVector velocity;	// Distance per second
			eae6320::Math::sVector acceleration;	// Distance per second^2
			eae6320::Math::sVector angularVelocity; // Radians per second (positive values rotate counter-clockwise, negative values rotate clockwise)
				
			bool rotationLocked[3];	// Lock rotation on x, y, z axis

			float dragCoefficient;	// Drag is the force that resists movement
			float mass;
			eae6320::Math::sVector force;
			bool gravityEnabled;
			float g;	// m/s^2
		};
	}
}

#include "RigidBody.inl"