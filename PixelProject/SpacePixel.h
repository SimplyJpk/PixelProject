#pragma once
#include "BasePixel.h"
#include "WorldDataHandler.h"

class SpacePixel : public BasePixel
{
public:
		virtual E_PixelType GetType() override { return E_PixelType::Space; }

		SpacePixel() {
				name = "Space";
				ColourCount = 1;
				// Browns
				TypeColours[0] = 0x00000000;
		}

		bool isUpdateable() { return false; }

private:
};