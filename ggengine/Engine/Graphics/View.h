#ifndef EAE6320_VIEW_H
#define EAE6320_VIEW_H

// Includes
//=========
#if defined( EAE6320_PLATFORM_D3D )
#include "Direct3D/Includes.h"
#elif defined( EAE6320_PLATFORM_GL )
#include "OpenGL/Includes.h"
#endif

#include "Configuration.h"

#include <Engine/Results/Results.h>
#include <Engine/Concurrency/cEvent.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
#include <Engine/Windows/Includes.h>
#endif

namespace eae6320
{
	namespace Graphics
	{

		struct sInitializationParameters
		{
#if defined( EAE6320_PLATFORM_WINDOWS )
			HWND mainWindow = NULL;
	#if defined( EAE6320_PLATFORM_D3D )
				uint16_t resolutionWidth = 0, resolutionHeight = 0;
	#elif defined( EAE6320_PLATFORM_GL )
				HINSTANCE thisInstanceOfTheApplication = NULL;
	#endif
#endif
		};

		class View
		{
			public:

				cResult		InitializeViews (const sInitializationParameters & i_initializationParameters);
				void		PrepareViews(unsigned int pHexCode);
				void		SwapBuffer();
				void		CleanUp();


			private:

#if defined( EAE6320_PLATFORM_D3D )

				// In Direct3D "views" are objects that allow a texture to be used a particular way:
				// A render target view allows a texture to have color rendered to it
				ID3D11RenderTargetView* s_renderTargetView = nullptr;
				// A depth/stencil view allows a texture to have depth rendered to it
				ID3D11DepthStencilView* s_depthStencilView = nullptr;
#endif
		};
	}
}

#endif