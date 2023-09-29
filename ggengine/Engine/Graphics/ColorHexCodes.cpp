#include "ColorHexCodes.h"
using namespace eae6320::Graphics;

tRGBA eae6320::Graphics::GetRGBAFromHex(unsigned int pHexCode)
{
	tRGBA retVal{};

	// Convert hex color #AARRGGBB to OpenGL/D3D RGBA format

	float red = (float) (pHexCode & 0xFF000000);
	float green = (float) (pHexCode & 0x00FF0000);
	float blue = (float) (pHexCode & 0x0000FF00);
	float alpha = (float) (pHexCode & 0x000000FF);

	// Set the clear color
	retVal.R = red;
	retVal.G = green;
	retVal.B = blue;
	retVal.A = alpha;

	return retVal;
}