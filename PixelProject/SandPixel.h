#pragma once
#include "BasePixel.h"
class SandPixel : public BasePixel
{
public:
		SandPixel() {
				name = "Sand";
				ColourCount = 3;
				// Yellows
				TypeColours[0] = 0xe2d9ae00;
				TypeColours[1] = 0xe8bb9a00;
				TypeColours[2] = 0xEDC9AF00;
		}

private:
};
