#pragma once
#include "BasePixel.h"

class SpacePixel : public BasePixel
{
public:
		SpacePixel() {
				name = "Space";
				ColourCount = 1;
				// Browns
				TypeColours[0] = 0x00000000;
		}

		bool isUpdateable() { return false; }

private:
};