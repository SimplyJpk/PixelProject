#pragma once
#include <SDL.h>

#define MAX_PIXEL_COLOUR_COUNT 10

class BasePixel
{
public:
		virtual const char* Name() { return name; }
		virtual bool isUpdateable() { return true; }

		const char* name = "UNKNOWN";

		short PixelIndex = -1;
		short ColourCount = 0;
		Uint32 TypeColours[MAX_PIXEL_COLOUR_COUNT]{ 0 };

		virtual bool Update() {

				return false;
		};
};



