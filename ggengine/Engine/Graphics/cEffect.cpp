// Includes
//=========

#include "cEffect.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Results/Results.h>
#include <new>

//#if defined( EAE6320_PLATFORM_GL )
//GLuint eae6320::Graphics::cEffect::s_programId{ 0 };
//#endif

// Platform independent interfaces

eae6320::cResult eae6320::Graphics::cEffect::Load(const std::string pVertexShaderPath, const std::string pFragmentShaderPath, cEffect*& o_Effect)
{
	sizeof(cEffect);
	auto result = Results::Success;

	cEffect* newEffect = nullptr;
	cScopeGuard scopeGuard([&o_Effect, &result, &pVertexShaderPath, &pFragmentShaderPath, &newEffect]
		{
			if (result)
			{
				EAE6320_ASSERT(newEffect != nullptr);
				o_Effect = newEffect;
			}
			else
			{
				if (newEffect)
				{
					newEffect->DecrementReferenceCount();
					newEffect = nullptr;
				}
				o_Effect = nullptr;
			}
		});

	// Allocate an effect
	{
		newEffect = new (std::nothrow) cEffect();
		if (!newEffect)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the effect");
			Logging::OutputError("Failed to allocate memory for the effect");
			return result;
		}
	}

	// Initialize the effect with shaders
	if (!(result = newEffect->InitializeShadingData(pVertexShaderPath, pFragmentShaderPath)))
	{
		EAE6320_ASSERTF(false, "Initialization of new effect failed");
		return result;
	}

	return result;
}

eae6320::Graphics::cEffect::~cEffect()
{
	EAE6320_ASSERT(m_referenceCount == 0);
	eae6320::cResult result = eae6320::Results::Success;
	CleanUp(result);
	EAE6320_ASSERT(result);
}

eae6320::cResult eae6320::Graphics::cEffect::InitializeShadingData (const std::string pVertexShaderPath, const std::string pFragmentShaderPath)
{
	auto result = eae6320::Results::Success;

	if (!(result = eae6320::Graphics::cShader::Load(pVertexShaderPath,
		s_vertexShader, eae6320::Graphics::eShaderType::Vertex)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without vertex shader");
		return result;
	}
	if (!(result = eae6320::Graphics::cShader::Load(pFragmentShaderPath,
		s_fragmentShader, eae6320::Graphics::eShaderType::Fragment)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without fragment shader");
		return result;
	}
	{
		constexpr auto renderStateBits = []
		{
			uint8_t renderStateBits = 0;

			eae6320::Graphics::RenderStates::DisableAlphaTransparency(renderStateBits);
			eae6320::Graphics::RenderStates::DisableDepthTesting(renderStateBits);
			eae6320::Graphics::RenderStates::DisableDepthWriting(renderStateBits);
			eae6320::Graphics::RenderStates::DisableDrawingBothTriangleSides(renderStateBits);

			return renderStateBits;
		}();
		if (!(result = s_renderState.Initialize(renderStateBits)))
		{
			EAE6320_ASSERTF(false, "Can't initialize shading data without render state");
			return result;
		}
	}

#if defined( EAE6320_PLATFORM_GL )

	// OpenGL specific initialization code
	
	// Create a program
	eae6320::cScopeGuard scopeGuard_program([&result, this]
		{
			if (!result)
			{
				if (s_programId != 0)
				{
					glDeleteProgram(s_programId);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						eae6320::Logging::OutputError("OpenGL failed to delete the program: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
					}
					s_programId = 0;
				}
			}
		});
	{
		EAE6320_ASSERT(s_programId == 0);
		s_programId = glCreateProgram();
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to create a program: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
		else if (s_programId == 0)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERT(false);
			eae6320::Logging::OutputError("OpenGL failed to create a program");
			return result;
		}
	}
	// Attach the shaders to the program
	{
		// Vertex
		{
			EAE6320_ASSERT((s_vertexShader != nullptr) && (s_vertexShader->m_shaderId != 0));
			glAttachShader(s_programId, s_vertexShader->m_shaderId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to attach the vertex shader to the program: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
		// Fragment
		{
			EAE6320_ASSERT((s_fragmentShader != nullptr) && (s_fragmentShader->m_shaderId != 0));
			glAttachShader(s_programId, s_fragmentShader->m_shaderId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to attach the fragment shader to the program: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
	}
	// Link the program
	{
		glLinkProgram(s_programId);
		const auto errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			// Get link info
			// (this won't be used unless linking fails
			// but it can be useful to look at when debugging)
			std::string linkInfo;
			{
				GLint infoSize;
				glGetProgramiv(s_programId, GL_INFO_LOG_LENGTH, &infoSize);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					if (infoSize > 0)
					{
						auto* const info = new (std::nothrow) GLchar[infoSize];
						if (info)
						{
							eae6320::cScopeGuard scopeGuard_info([info]
								{
									delete[] info;
								});
							constexpr GLsizei* const dontReturnLength = nullptr;
							glGetProgramInfoLog(s_programId, static_cast<GLsizei>(infoSize), dontReturnLength, info);
							const auto errorCode = glGetError();
							if (errorCode == GL_NO_ERROR)
							{
								linkInfo = info;
							}
							else
							{
								result = eae6320::Results::Failure;
								EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
								eae6320::Logging::OutputError("OpenGL failed to get link info of the program: %s",
									reinterpret_cast<const char*>(gluErrorString(errorCode)));
								return result;
							}
						}
						else
						{
							result = eae6320::Results::OutOfMemory;
							EAE6320_ASSERTF(false, "Couldn't allocate memory for the program link info");
							eae6320::Logging::OutputError("Failed to allocate memory for the program link info");
							return result;
						}
					}
				}
				else
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to get the length of the program link info: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Check to see if there were link errors
			GLint didLinkingSucceed;
			{
				glGetProgramiv(s_programId, GL_LINK_STATUS, &didLinkingSucceed);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					if (didLinkingSucceed == GL_FALSE)
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF(false, linkInfo.c_str());
						eae6320::Logging::OutputError("The program failed to link: %s",
							linkInfo.c_str());
						return result;
					}
				}
				else
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to find out if linking of the program succeeded: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
		}
		else
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to link the program: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}

	return result;

#elif defined (EAE6320_PLATFORM_D3D)
	return result;
#endif
}

void eae6320::Graphics::cEffect::CleanUp (eae6320::cResult& pResult)
{
	if (s_vertexShader)
	{
		s_vertexShader->DecrementReferenceCount();
		s_vertexShader = nullptr;
	}
	if (s_fragmentShader)
	{
		s_fragmentShader->DecrementReferenceCount();
		s_fragmentShader = nullptr;
	}

#if defined( EAE6320_PLATFORM_GL )
	if (eae6320::Graphics::cEffect::s_programId != 0)
	{
		glDeleteProgram(eae6320::Graphics::cEffect::s_programId);
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			if (pResult)
			{
				pResult = eae6320::Results::Failure;
			}
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to delete the program: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		eae6320::Graphics::cEffect::s_programId = 0;
	}
#endif
}