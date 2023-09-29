/*
	This file declares the external interface for the graphics system
*/

#ifndef EAE6320_GRAPHICS_H
#define EAE6320_GRAPHICS_H

// Includes
//=========

#include "Configuration.h"

#include <cstdint>
#include <Engine/Results/Results.h>
#include <Engine/Concurrency/cEvent.h>
#include <Engine/Assets/Camera.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>
#endif

#include "cConstantBuffer.h"
#include "ConstantBufferFormats.h"
#include "VertexMesh.h"
#include "cEffect.h"
#include "View.h"
#include "ColorHexCodes.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct MeshEffectData {

			eae6320::Graphics::VertexMesh* mesh = nullptr;
			eae6320::Graphics::cEffect* effect = nullptr;
		};

		struct ObjectRenderData {

			eae6320::Graphics::VertexMesh* mesh = nullptr;
			eae6320::Graphics::cEffect* effect = nullptr;
			eae6320::Graphics::ConstantBufferFormats::sDrawCall drawData;
		};


		// Submission
		//-----------

		// These functions should be called from the application (on the application loop thread)

		// As the class progresses you will add your own functions for submitting data,
		// but the following is an example (that gets called automatically)
		// of how the application submits the total elapsed times
		// for the frame currently being submitted
		void SubmitElapsedTime( const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime );

		// When the application is ready to submit data for a new frame
		// it should call this before submitting anything
		// (or, said another way, it is not safe to submit data for a new frame
		// until this function returns successfully)
		cResult WaitUntilDataForANewFrameCanBeSubmitted( const unsigned int i_timeToWait_inMilliseconds );
		// When the application has finished submitting data for a frame
		// it must call this function
		cResult SignalThatAllDataForAFrameHasBeenSubmitted();

		// Render
		//-------

		// This is called (automatically) from the main/render thread.
		// It will render a submitted frame as soon as it is ready
		// (i.e. as soon as SignalThatAllDataForAFrameHasBeenSubmitted() has been called)
		void RenderFrame();

		// Initialize / Render / Clean Up
		//----------------------


		cResult		Initialize					(const sInitializationParameters& i_initializationParameters);
		void		Render						();
		cResult		CleanUp						();

		void		SetBackgroundColor			(unsigned int pHexCode);
		void		SetRenderableObjects		(ObjectRenderData pMeshEffect[], uint16_t pNumMeshEffectPairs);
		void		SetCameraTransforms         (Math::cMatrix_transformation pWorldToCameraTransform, Math::cMatrix_transformation pCameraToProjectedTransform);
	}
}

#endif	// EAE6320_GRAPHICS_H
