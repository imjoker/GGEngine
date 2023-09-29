// Includes
//=========

#include "Includes.h"
#include "../cEffect.h"

// Bind and render the shading data
void eae6320::Graphics::cEffect::Render()
{

	{
		EAE6320_ASSERT(s_programId != 0);
		glUseProgram(s_programId);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
	}
	// Render state
	{
		s_renderState.Bind();
	}
}