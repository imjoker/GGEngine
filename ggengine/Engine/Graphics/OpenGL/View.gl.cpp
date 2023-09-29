// Includes
//=========

#include "../View.h"

#include "../sContext.h"
#include "../ConstantBufferFormats.h"
#include "../cRenderState.h"
#include "../cShader.h"
#include "../cVertexFormat.h"
#include "../VertexFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cEvent.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>
#include <utility>

eae6320::cResult eae6320::Graphics::View::InitializeViews (const sInitializationParameters & i_initializationParameters)
{
	// nothing to do here for OpenGL

	return eae6320::Results::Success;

}

void eae6320::Graphics::View::PrepareViews(unsigned int pHexCode)
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		// Black is usually used
		{
			// Get the user-set background color
			tRGBA bgColor = GetRGBAFromHex(pHexCode);

			glClearColor(bgColor.R, bgColor.G, bgColor.B, bgColor.A);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
		{
			constexpr GLbitfield clearColor = GL_COLOR_BUFFER_BIT;
			glClear(clearColor);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
	}
	// In addition to the color buffer there is also a hidden image called the "depth buffer"
	// which is used to make it less important which order draw calls are made.
	// It must also be "cleared" every frame just like the visible color buffer.
	{
		{
			glDepthMask(GL_TRUE);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			constexpr GLclampd clearToFarDepth = 1.0;
			glClearDepth(clearToFarDepth);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
		{
			constexpr GLbitfield clearDepth = GL_DEPTH_BUFFER_BIT;
			glClear(clearDepth);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
	}
}

void eae6320::Graphics::View::SwapBuffer()
{
	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (or "swapped" with the "front buffer", which is the image that is actually being displayed)
	const auto deviceContext = sContext::g_context.deviceContext;
	EAE6320_ASSERT(deviceContext != NULL);
	const auto glResult = SwapBuffers(deviceContext);
	EAE6320_ASSERT(glResult != FALSE);
}

void eae6320::Graphics::View::CleanUp()
{

}