#pragma once


namespace eae6320 {

	namespace Graphics {

		struct tRGBA {

			float R;
			float G;
			float B;
			float A;
		};

		constexpr unsigned int Red = 0xFF0000FF;       // Red (#FF0000)
		constexpr unsigned int Green = 0x00FF00FF;     // Green (#00FF00)
		constexpr unsigned int Blue = 0x0000FFFF;      // Blue (#0000FF)
		constexpr unsigned int Yellow = 0xFFFF00FF;    // Yellow (#FFFF00)
		constexpr unsigned int Cyan = 0x00FFFFFF;      // Cyan (#00FFFF)
		constexpr unsigned int Magenta = 0xFF00FFFF;   // Magenta (#FF00FF)
		constexpr unsigned int White = 0xFFFFFFFF;     // White (#FFFFFF)
		constexpr unsigned int Black = 0x000000FF;     // Black (#000000)
		constexpr unsigned int Orange = 0xFFA500FF;    // Orange (#FFA500)
		constexpr unsigned int Purple = 0x800080FF;    // Purple (#800080)
		constexpr unsigned int Pink = 0xFFC0CBFF;      // Pink (#FFC0CB)
		constexpr unsigned int Brown = 0xA52A2AFF;     // Brown (#A52A2A)
		constexpr unsigned int Gray = 0x808080FF;      // Gray (#808080)
		constexpr unsigned int LightGray = 0xC0C0C0FF; // Light Gray (#C0C0C0)
		constexpr unsigned int DarkGray = 0x404040FF;  // Dark Gray (#404040)

		tRGBA GetRGBAFromHex(unsigned int pHexCode);

	}
}