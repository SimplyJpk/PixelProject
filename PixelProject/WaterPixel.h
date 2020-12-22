#pragma once
#include "BasePixel.h"
#include "WorldDataHandler.h"

class WaterPixel : public BasePixel
{
public:
		virtual E_PixelType GetType() override { return E_PixelType::Water; }

		WaterPixel() {
				name = "Water";
				ColourCount = 3;
				// Browns
				TypeColours[0] = 0x0000FF00;
				TypeColours[1] = 0x2e2ee600;
				TypeColours[2] = 0x1818f300;
		}

private:
};