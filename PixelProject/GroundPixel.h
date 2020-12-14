#pragma once
#include "BasePixel.h"

class GroundPixel : public BasePixel
{
public:
		GroundPixel() {
				ColourCount = 3;
				// Browns
				TypeColours[0] = 0x964b0000;
				TypeColours[1] = 0x96710000;
				TypeColours[2] = 0x96260000;
		}

		const char* Name()
		{
				return "Ground";
		}

private:
};