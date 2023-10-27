/*
	This is the standard vertex shader

	A vertex shader is responsible for two things:
		* Telling the GPU where the vertex (one of the three in a triangle) should be drawn on screen in a given window
			* The GPU will use this to decide which fragments (i.e. pixels) need to be shaded for a given triangle
		* Providing any data that a corresponding fragment shader will need
			* This data will be interpolated across the triangle and thus vary for each fragment of a triangle that gets shaded
*/

#include <Shaders/shaders.inc>

// Constant Buffers
//=================

DeclareConstantBuffer (g_constantBuffer_frame, 0)
{
	float4x4 g_transform_worldToCamera;
	float4x4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For float4 alignment
	float2 g_padding;
};

DeclareConstantBuffer (g_constantBuffer_drawCall, 2)
{
	float4x4 g_transform_localToWorld;
};

// Entry Point
//============

#if defined( EAE6320_PLATFORM_GL )	
layout( location = 0 ) in vec3 i_vertexPosition_local;
#endif

void main(
	#if defined( EAE6320_PLATFORM_D3D )
	// Input
	//======

	// The "semantics" (the keywords in all caps after the colon) are arbitrary,
	// but must match the C call to CreateInputLayout()

	// These values come from one of the VertexFormats::sVertex_mesh that the vertex buffer was filled with in C code
	in const float3 i_vertexPosition_local : POSITION,

	// Output
	//=======

	// An SV_POSITION value must always be output from every vertex shader
	// so that the GPU can figure out which fragments need to be shaded
	out float4 o_vertexPosition_projected : SV_POSITION
	#endif
)
{
	// Transform the local vertex into world space
	float4 vertexPosition_world;
	{
		// This will be done in a future assignment.
		// For now, however, local space is treated as if it is the same as world space.
		float4 vertexPosition_local = float4( i_vertexPosition_local, 1.0 );
		vertexPosition_world = mul (g_transform_localToWorld, vertexPosition_local);
	}
	// Calculate the position of this vertex projected onto the display
	{
		// Transform the vertex from world space into camera space
		float4 vertexPosition_camera = mul( g_transform_worldToCamera, vertexPosition_world );
		// Project the vertex from camera space into projected space
		o_vertexPosition_projected = mul( g_transform_cameraToProjected, vertexPosition_camera );
	}
}
