// represents a 3D object's geometry, and is internal to your Graphics system

#ifndef EAE6320_GRAPHICS_VERTEXMESH_H
#define EAE6320_GRAPHICS_VERTEXMESH_H

// Includes
//=========

#include "cVertexFormat.h"
#include "cConstantBuffer.h"
#include "VertexFormats.h"

#include <Engine/Results/Results.h>
#include <Engine/Assets/ReferenceCountedAssets.h>


namespace eae6320
{
	namespace Graphics
	{
		struct tGeomertryInitData {

			eae6320::Graphics::VertexFormats::sVertex_mesh* vertexData;
			uint16_t* indexData;
			uint16_t numVertices;
			uint16_t numIndexes;
		};

		// Mesh for 3D Geometry

		class VertexMesh {

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(VertexMesh)

			public:

				EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()

				static cResult		Load							(tGeomertryInitData& pInitData, VertexMesh*& o_Mesh);

				void				DrawGeometry					();

			private:

				eae6320::cResult	InitializeGeometry				(tGeomertryInitData & pInitData);
				void				CleanUp							(eae6320::cResult& pResult);

				VertexMesh() = default;
				~VertexMesh();

	constexpr	void				ConvertLeftHandedToRightHanded	(uint16_t* indexBuffer, uint16_t pNumIndexes);

			// Geometry Data
			//--------------

#if defined( EAE6320_PLATFORM_D3D )

				eae6320::Graphics::cVertexFormat* s_vertexFormat = nullptr;
				// A vertex buffer holds the data for each vertex
				ID3D11Buffer* s_vertexBuffer = nullptr;
				// Index buffer
				ID3D11Buffer* s_indexBuffer = nullptr;

#elif defined( EAE6320_PLATFORM_GL )

				// A vertex buffer holds the data for each vertex
				GLuint s_vertexBufferId = 0;
				// A vertex array encapsulates the vertex data as well as the vertex input layout
				GLuint s_vertexArrayId = 0;
				// A index buffer holds the index to refer to from vertex buffer for rendering
				GLuint s_IndexBufferId = 0;

#endif
				unsigned int numIndexes = 0;
				EAE6320_ASSETS_DECLAREREFERENCECOUNT()
		};
	}
}

constexpr void eae6320::Graphics::VertexMesh::ConvertLeftHandedToRightHanded(uint16_t* indexBuffer, uint16_t pNumIndexes)
{
	for (uint16_t i = 0; i < pNumIndexes; i += 3) {

		uint16_t temp = indexBuffer[i + 1];
		indexBuffer[i + 1] = indexBuffer[i + 2];
		indexBuffer[i + 2] = temp;
	}
}

#endif