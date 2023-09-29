#include "GameObject.h"

using namespace eae6320;

eae6320::Assets::GameObject::GameObject(Graphics::VertexMesh* pMesh, Graphics::cEffect* pEffect, Physics::sRigidBodyState& pRigidBody)
{
    m_mesh = pMesh;
    m_effect = pEffect;
    m_rigidBody = pRigidBody;

    m_mesh->IncrementReferenceCount();
    m_effect->IncrementReferenceCount();
}

eae6320::Assets::GameObject::~GameObject()
{
    m_mesh->DecrementReferenceCount();
    m_effect->DecrementReferenceCount();
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
    m_rigidBody.Update(i_secondCountToIntegrate);
}

void eae6320::Assets::GameObject::AddVelocity(Math::sVector pVelocity) noexcept
{
    m_rigidBody.velocity += pVelocity;
}

Math::cMatrix_transformation eae6320::Assets::GameObject::GetTransformLocalToWorld(float i_secondCountToExtrapolate) const noexcept
{
    return m_rigidBody.PredictFutureTransform(i_secondCountToExtrapolate);
}

Graphics::VertexMesh* eae6320::Assets::GameObject::GetMesh() const noexcept
{
    return m_mesh;
}

Graphics::cEffect* eae6320::Assets::GameObject::GetEffect() const noexcept
{
    return m_effect;
}
