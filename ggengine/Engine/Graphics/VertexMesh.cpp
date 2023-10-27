#include "VertexMesh.h"
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Logging/Logging.h>
#include <new>
#include <iostream>

eae6320::Graphics::VertexMesh* eae6320::Graphics::VertexMesh::CreateMeshFromFile(const std::string& pFilePath)
{
	VertexMesh* mesh = nullptr;
	lua_State* luaState = nullptr;
	VertexFormats::sVertex_mesh* vertexData;
	uint16_t* indices;
	Graphics::tGeomertryInitData MeshInitData;

	eae6320::cScopeGuard scopeGuard_onExit([&luaState]
	{
		lua_pop(luaState, 1);

		if (luaState)
		{
			// If I haven't made any mistakes
			// there shouldn't be anything on the stack
			// regardless of any errors
			EAE6320_ASSERT(lua_gettop(luaState) == 0);

			lua_close(luaState);
			luaState = nullptr;
		}
	});

	eae6320::cScopeGuard scopeGuard_MeshInitData([&MeshInitData]
	{
			delete[] MeshInitData.indexData;
			delete[] MeshInitData.vertexData;
	});

	if (LoadLuaFile(pFilePath, luaState) != eae6320::Results::Success)
		return mesh;

	// table is now at index - 1

	// Right now the asset table is at -1.
	// After the following table operation it will be at -2
	// and the "vertices" table will be at -1:
	constexpr auto* const key = "vertices";
	lua_pushstring(luaState, key);
	lua_gettable(luaState, -2);

	// We can create a scope guard immediately as soon as the new table has been pushed
	// to guarantee that it will be popped when we are done with it:
	//eae6320::cScopeGuard scopeGuard_popVertices([luaState]
	//{
	//	lua_pop(luaState, 1);
	//});

	// Additionally, I try not to do any further stack manipulation in this function
	// and call other functions that assume the "vertices" table is at -1
	// but don't know or care about the rest of the stack
	if (lua_istable(luaState, -1))
	{
		if (!(LoadVerticesFromTable(*luaState, vertexData, MeshInitData.numVertices)))
			return mesh;
	}
	else
	{
		std::cerr << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(luaState, -1) << ")" << std::endl;
		return mesh;
	}

	// pop the vertex table
	lua_pop(luaState, 1);

	constexpr auto* const key2 = "indices";
	lua_pushstring(luaState, key2);
	lua_gettable(luaState, -2);

	eae6320::cScopeGuard scopeGuard_popIndices([luaState]
	{
		lua_pop(luaState, 1);
	});

	if (lua_istable(luaState, -1))
	{
		if (!(LoadIndicesFromTable(*luaState, indices, MeshInitData.numIndexes)))
			return mesh;
	}
	else
	{
		std::cerr << "The value at \"" << key2 << "\" must be a table "
			"(instead of a " << luaL_typename(luaState, -1) << ")" << std::endl;
		return mesh;
	}

	MeshInitData.vertexData = vertexData;
	MeshInitData.indexData = indices;

	if (!(eae6320::Graphics::VertexMesh::Load(MeshInitData, mesh)))
	{
		EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
		return mesh;
	}

	return mesh;
}

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

eae6320::cResult eae6320::Graphics::VertexMesh::LoadLuaFile(const std::string& pFilePath, lua_State*& pLuaState)
{
	auto result = eae6320::Results::Success;

	// Create a new Lua state
	lua_State* luaState = nullptr;
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			result = eae6320::Results::OutOfMemory;
			std::cerr << "Failed to create a new Lua state" << std::endl;
			return result;
		}
	}

	// Load the asset file as a "chunk",
	// meaning there will be a callable function at the top of the stack
	const auto stackTopBeforeLoad = lua_gettop(luaState);
	{
		const auto luaResult = luaL_loadfile(luaState, pFilePath.c_str());
		if (luaResult != LUA_OK)
		{
			result = eae6320::Results::Failure;
			std::cerr << lua_tostring(luaState, -1) << std::endl;
			// Pop the error message
			lua_pop(luaState, 1);
			return result;
		}
	}
	// Execute the "chunk", which should load the asset
	// into a table at the top of the stack
	{
		constexpr int argumentCount = 0;
		constexpr int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
		constexpr int noMessageHandler = 0;
		const auto luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
		if (luaResult == LUA_OK)
		{
			// A well-behaved asset file will only return a single value
			const auto returnedValueCount = lua_gettop(luaState) - stackTopBeforeLoad;
			if (returnedValueCount == 1)
			{
				// A correct asset file _must_ return a table
				if (!lua_istable(luaState, -1))
				{
					result = eae6320::Results::InvalidFile;
					std::cerr << "Asset files must return a table (instead of a " <<
						luaL_typename(luaState, -1) << ")" << std::endl;
					// Pop the returned non-table value
					lua_pop(luaState, 1);
					return result;
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				std::cerr << "Asset files must return a single table (instead of " <<
					returnedValueCount << " values)" << std::endl;
				// Pop every value that was returned
				lua_pop(luaState, returnedValueCount);
				return result;
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			std::cerr << lua_tostring(luaState, -1) << std::endl;
			// Pop the error message
			lua_pop(luaState, 1);
			return result;
		}
	}

	// If this code is reached the asset file was loaded successfully,
	pLuaState = luaState;

	return result;
}

eae6320::cResult eae6320::Graphics::VertexMesh::LoadVerticesFromTable(lua_State& io_luaState, VertexFormats::sVertex_mesh*& o_Vertices, uint16_t& pNumVertices)
{
	auto result = eae6320::Results::Success;

	VertexFormats::sVertex_mesh* vertexData;

	eae6320::cScopeGuard scopeGuard_clearVertexOnFailure([result, vertexData]
	{
		if (result != eae6320::Results::Success)
			delete (vertexData);
	});

	std::cout << "Iterating through every vertex:" << std::endl;
	const auto vertexCount = luaL_len(&io_luaState, -1);
	pNumVertices = (uint16_t) vertexCount;

	vertexData = new VertexFormats::sVertex_mesh[pNumVertices];
	int ndx = 0;

	for (uint16_t i = 1; i <= pNumVertices; ++i)
	{
		float values[3]{};

		lua_pushinteger(&io_luaState, i);
		lua_gettable(&io_luaState, -2);

		//// Get the row table at index i
		//lua_rawgeti(&io_luaState, -1, i);

		// Check if it's a table (inner array)
		if (lua_istable(&io_luaState, -1)) {

			// Iterate through each number in the row
			for (int j = 1; j <= 3; ++j) {

				lua_pushinteger(&io_luaState, j);
				lua_gettable(&io_luaState, -2);

				//lua_rawgeti(&io_luaState, -1, j);

				// Check if it's a number
				if (lua_isnumber(&io_luaState, -1))

					values[j - 1] = (float) lua_tonumber(&io_luaState, -1);
				else
					result = eae6320::Results::Failure;

				lua_pop(&io_luaState, 1); // Pop the number value
			}

			vertexData[i-1] = values;
		} else
			result = eae6320::Results::Failure;

		lua_pop(&io_luaState, 1); // Pop the row table
	}

	o_Vertices = vertexData;
	return result;
}

eae6320::cResult eae6320::Graphics::VertexMesh::LoadIndicesFromTable(lua_State& io_luaState, uint16_t*& o_Indices, uint16_t& pNumIndices)
{
	auto result = eae6320::Results::Success;
	uint16_t* indices;

	eae6320::cScopeGuard scopeGuard_clearVertexOnFailure([result, indices]
	{
		if (result != eae6320::Results::Success)
			delete (indices);
	});

	std::cout << "Iterating through every index:" << std::endl;
	const auto numIndices = luaL_len(&io_luaState, -1);
	pNumIndices = (uint16_t) numIndices;

	indices = new uint16_t[pNumIndices];

	for (uint16_t i = 1; i <= pNumIndices; ++i)
	{
		lua_pushinteger(&io_luaState, i);
		lua_gettable(&io_luaState, -2);

		// Check if it's a number
		if (lua_isinteger(&io_luaState, -1))

			indices[i - 1] = (uint16_t) lua_tointeger(&io_luaState, -1);
		else
			result = eae6320::Results::Failure;

		lua_pop(&io_luaState, 1); // Pop the number value
	}

	o_Indices = indices;

	return result;
}

eae6320::Graphics::VertexMesh::~VertexMesh()
{
	EAE6320_ASSERT(m_referenceCount == 0);
	eae6320::cResult result = eae6320::Results::Success;
	CleanUp(result);
	EAE6320_ASSERT(result);
}     