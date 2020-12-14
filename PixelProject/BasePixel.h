#pragma once
#include <SDL.h>

#define MAX_PIXEL_COLOUR_COUNT 10

class BasePixel
{
public:
		virtual const char* Name() = 0;
		short ColourCount = 0;
		Uint32 TypeColours [MAX_PIXEL_COLOUR_COUNT]{ 0 };

		bool isUpdateable = true;
		bool isMoveable = true;
};

