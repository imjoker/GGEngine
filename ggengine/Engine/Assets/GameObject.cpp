#include "GameObject.h"
#include <math.h>

using namespace eae6320;

eae6320::Assets::GameObject::GameObject(Graphics::VertexMesh* pMesh, Graphics::cEffect* pEffect, Math::sVector pPosition, Math::sVector pColliderExtents)
{
    m_mesh = pMesh;
    m_effect = pEffect;
    m_position = pPosition;

    m_pRigidBody = new ChrisZ::Physics::RigidBody(this);
    m_pCollider = new ChrisZ::Physics::BoxCollider(Math::sVector(), pColliderExtents, this);

    m_mesh->IncrementReferenceCount();
    m_effect->IncrementReferenceCount();

    m_pRigidBody->SetGravityEnabled(false);
}

eae6320::Assets::GameObject::~GameObject()
{
    m_mesh->DecrementReferenceCount();
    m_effect->DecrementReferenceCount();

    if (m_pRigidBody)
        delete m_pRigidBody;

    if (m_pCollider)
		delete m_pCollider;
}

void eae6320::Assets::GameObject::SetMesh(Graphics::VertexMesh* pMesh) noexcept
{
    m_mesh = pMesh;
}

void eae6320::Assets::GameObject::SetEffect(Graphics::cEffect* pEffect) noexcept
{
    m_effect = pEffect;
}

void eae6320::Assets::GameObject::Update(float i_secondCountToIntegrate) noexcept
{
    m_pRigidBody->Update(i_secondCountToIntegrate);
}

void eae6320::Assets::GameObject::Rotate(const Math::cQuaternion& rotation) noexcept
{
    SetOrientation(rotation);
}

void eae6320::Assets::GameObject::SetVelocity(Math::sVector pVelocity) noexcept
{
	m_pRigidBody->SetVelocity(pVelocity);
}

void eae6320::Assets::GameObject::AddVelocity(Math::sVector pVelocity) noexcept
{
    m_pRigidBody->SetVelocity(m_pRigidBody->GetVelocity() + pVelocity);
}

// Set the velocity to reach the target location with a specified speed
void eae6320::Assets::GameObject::SetVelocityToReachTarget(const Math::sVector& target) noexcept
{

    if (HasReachedTarget(target))
		return;

    // Calculate the direction vector to the target
    Math::sVector direction = (target - m_position).GetNormalized();

    // Set the velocity by scaling the direction vector with the speed
    m_pRigidBody->SetVelocity (direction * 2.5f);
}

Math::sVector eae6320::Assets::GameObject::GetPosition() const noexcept 
{ 
    return m_position; 
}

void eae6320::Assets::GameObject::SetPosition(const Math::sVector& i_position)
{
	m_position = i_position;

    if (m_pCollider)
		m_pCollider->SetCenter(i_position + m_pCollider->GetCenterOffset());
}

Math::cQuaternion eae6320::Assets::GameObject::GetOrientation() const
{
    return m_orientation;
}

void eae6320::Assets::GameObject::SetOrientation(const Math::cQuaternion& i_orientation)
{
    m_orientation = i_orientation;
}

// Check if the object has reached the target
bool eae6320::Assets::GameObject::HasReachedTarget(const Math::sVector& target) const 
{ 
    return (std::abs(m_position.x - target.x) < 0.5f) && (std::abs(m_position.y - target.y) < 0.5f) && (std::abs(m_position.z - target.z) < 0.5f);
}


Math::cMatrix_transformation eae6320::Assets::GameObject::GetTransformLocalToWorld(float i_secondCountToExtrapolate) const noexcept
{
    return m_pRigidBody->PredictFutureTransform(i_secondCountToExtrapolate);
}

Graphics::VertexMesh* eae6320::Assets::GameObject::GetMesh() const noexcept
{
    return m_mesh;
}

Graphics::cEffect* eae6320::Assets::GameObject::GetEffect() const noexcept
{
    return m_effect;
}
