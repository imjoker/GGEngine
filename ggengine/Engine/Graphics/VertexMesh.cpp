#include "VertexMesh.h"
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Logging/Logging.h>
#include <new>

eae6320::cResult eae6320::Graphics::VertexMesh::Load(tGeomertryInitData& pInitData, VertexMesh*& o_Mesh)
{
	auto result = Results::Success;

	VertexMesh* newMesh = nullptr;
	cScopeGuard scopeGuard([&o_Mesh, &result, &pInitData, &newMesh]
	{
		if (result)
		{
			EAE6320_ASSERT(newMesh != nullptr);
			o_Mesh = newMesh;
		}
		else
		{
			if (newMesh)
			{
				newMesh->DecrementReferenceCount();
				newMesh = nullptr;
			}
			o_Mesh = nullptr;
		}
	});

	// Allocate a mesh
	{
		newMesh = new (std::nothrow) VertexMesh();
		if (!newMesh)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the mesh");
			Logging::OutputError("Failed to allocate memory for the mesh");
			return result;
		}
	}

	// Initialize the platform-specific graphics API mesh geometry
	if (!(result = newMesh->InitializeGeometry(pInitData)))
	{
		EAE6320_ASSERTF(false, "Initialization of new mesh failed");
		return result;
	}

	return result;
}

eae6320::Graphics::VertexMesh::~VertexMesh()
{
	EAE6320_ASSERT(m_referenceCount == 0);
	eae6320::cResult result = eae6320::Results::Success;
	CleanUp(result);
	EAE6320_ASSERT(result);
}