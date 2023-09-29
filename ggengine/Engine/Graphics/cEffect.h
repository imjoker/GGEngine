#ifndef EAE6320_GRAPHICS_CEFFECT_H
#define EAE6320_GRAPHICS_CEFFECT_H

// Includes
//=========

#include "cShader.h"
#include "cRenderState.h"

#include <Engine/Results/cResult.h>
#include <Engine/Assets/ReferenceCountedAssets.h>
#include <string>

namespace eae6320
{
	namespace Graphics
	{
		// Interfaces for representation of Effect
		class cEffect {

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cEffect)

			public:

				EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()

				static cResult		Load					(const std::string pVertexShaderPath, const std::string pFragmentShaderPath, cEffect*& o_Mesh);

				void				Render					();

			private:

				cEffect() = default;
				~cEffect();

				eae6320::cResult	InitializeShadingData	(const std::string pVertexShaderPath, const std::string pFragmentShaderPath);
				void				CleanUp					(eae6320::cResult & pResult);


				// Shading Data
				//-------------

				eae6320::Graphics::cShader* s_vertexShader = nullptr;
				eae6320::Graphics::cShader* s_fragmentShader = nullptr;

#if defined( EAE6320_PLATFORM_GL )

		public:
				GLuint s_programId = 0;
#endif
		private:
				EAE6320_ASSETS_DECLAREREFERENCECOUNT()
				eae6320::Graphics::cRenderState s_renderState;

		};
	}
}

#endif
