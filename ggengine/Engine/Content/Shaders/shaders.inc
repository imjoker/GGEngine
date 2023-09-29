/*
	This file should be #included by all shaders
*/

// Version Information
//====================

#if defined( EAE6320_PLATFORM_GL )

	// GLSL shaders require the version to be #defined before anything else in the shader
	#version 420

#endif

// Constant Buffers
//=================

#if defined( EAE6320_PLATFORM_D3D )

	#define DeclareConstantBuffer( i_name, i_id ) cbuffer i_name : register( b##i_id )
	#define mat4 float4x4
	#define vec2 float2
	#define vec3 float3
	#define vec4 float4		
	#define mod(val1, val2) val1 % val2

#elif defined( EAE6320_PLATFORM_GL )

	#define DeclareConstantBuffer( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name
	#define float4x4 mat4
	#define float2 vec2
	#define float3 vec3
	#define float4 vec4	
	#define o_vertexPosition_projected gl_Position
	#define mul(transform1, transform2) transform1 * transform2
#endif
