// Includes
//=========

#include "Includes.h"
#include "../sContext.h"
#include "../cEffect.h"


// Bind and render the shading data
void eae6320::Graphics::cEffect::Render ()
{
	{
		auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;

		constexpr ID3D11ClassInstance* const* noInterfaces = nullptr;
		constexpr unsigned int interfaceCount = 0;
		// Vertex shader
		{
			EAE6320_ASSERT((s_vertexShader != nullptr) && (s_vertexShader->m_shaderObject.vertex != nullptr));
			direct3dImmediateContext->VSSetShader(s_vertexShader->m_shaderObject.vertex, noInterfaces, interfaceCount);
		}
		// Fragment shader
		{
			EAE6320_ASSERT((s_fragmentShader != nullptr) && (s_fragmentShader->m_shaderObject.vertex != nullptr));
			direct3dImmediateContext->PSSetShader(s_fragmentShader->m_shaderObject.fragment, noInterfaces, interfaceCount);
		}
	}
	// Render state
	{
		s_renderState.Bind();
	}
}