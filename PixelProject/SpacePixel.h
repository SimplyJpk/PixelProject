#pragma once
#include "BasePixel.h"
#include "WorldDataHandler.h"

class SpacePixel : public BasePixel
{
public:
		virtual const E_PixelType GetType() override { return E_PixelType::Space; }

		SpacePixel() {
				name = "Space";
				ColourCount = 1;
				// Browns
				TypeColours[0] = 0x00000000;

				PixelUpdateOrderCount = 0;
				InsertPixelUpdateOrder(0, std::vector<short>() = {});
		}

		bool isUpdateable() { return false; }

private:
};