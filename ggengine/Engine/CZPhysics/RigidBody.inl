#pragma once

#include "RigidBody.h"

inline void ChrisZ::Physics::RigidBody::AddForce(const eae6320::Math::sVector i_force)
{
	// Update force
	force += i_force;
}

inline void ChrisZ::Physics::RigidBody::AddImpulse(const eae6320::Math::sVector i_impulse)
{
	// Update velocity
	velocity += i_impulse / mass;
}

inline void ChrisZ::Physics::RigidBody::AddTorque(const eae6320::Math::sVector i_torque)
{
	// Update angular velocity according to rotationLocked[3]
	angularVelocity.x += !rotationLocked[0] * i_torque.x / mass;
	angularVelocity.y += !rotationLocked[1] * i_torque.y / mass;
	angularVelocity.z += !rotationLocked[2] * i_torque.z / mass;
}

// Getters and Setters
//====================

inline eae6320::Math::sVector ChrisZ::Physics::RigidBody::GetVelocity() const
{
	return velocity;
}

inline void ChrisZ::Physics::RigidBody::SetVelocity(eae6320::Math::sVector i_velocity)
{
	velocity = i_velocity;
}

inline eae6320::Math::sVector ChrisZ::Physics::RigidBody::GetAcceleration() const
{
	return acceleration;
}

inline void ChrisZ::Physics::RigidBody::SetAcceleration(eae6320::Math::sVector i_acceleration)
{
	acceleration = i_acceleration;
}

inline eae6320::Math::sVector ChrisZ::Physics::RigidBody::GetAngularVelocity() const
{
	return angularVelocity;
}

inline void ChrisZ::Physics::RigidBody::SetAngularVelocity(eae6320::Math::sVector i_angularVelocity)
{
	angularVelocity = i_angularVelocity;
}

inline bool ChrisZ::Physics::RigidBody::GetRotationLocked(int i_axis) const
{
	return rotationLocked[i_axis];
}

inline void ChrisZ::Physics::RigidBody::SetRotationLocked(bool i_rotationLocked_x, bool i_rotationLocked_y, bool i_rotationLocked_z)
{
	rotationLocked[0] = i_rotationLocked_x;
	rotationLocked[1] = i_rotationLocked_y;
	rotationLocked[2] = i_rotationLocked_z;
}

inline float ChrisZ::Physics::RigidBody::GetDragCoefficient() const
{
	return dragCoefficient;
}

inline void ChrisZ::Physics::RigidBody::SetDragCoefficient(float i_dragCoefficient)
{
	dragCoefficient = i_dragCoefficient;
}

inline float ChrisZ::Physics::RigidBody::GetMass() const
{
	return mass;
}

inline void ChrisZ::Physics::RigidBody::SetMass(float i_mass)
{
	mass = i_mass;
}

inline bool ChrisZ::Physics::RigidBody::GetGravityEnabled() const
{
	return gravityEnabled;
}

inline void ChrisZ::Physics::RigidBody::SetGravityEnabled(bool i_gravityEnabled)
{
	gravityEnabled = i_gravityEnabled;
}


inline eae6320::Math::cMatrix_transformation ChrisZ::Physics::RigidBody::PredictFutureTransform(const float i_secondCountToExtrapolate) const
{
	return eae6320::Math::cMatrix_transformation(PredictFutureOrientation(i_secondCountToExtrapolate), PredictFuturePosition(i_secondCountToExtrapolate));
}