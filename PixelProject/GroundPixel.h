#pragma once
#include "BasePixel.h"
#include "WorldDataHandler.h"

class GroundPixel : public BasePixel
{
public:
		virtual const E_PixelType GetType() override { return E_PixelType::Ground; }

		GroundPixel() {
				name = "Ground";
				ColourCount = 3;
				// Browns
				TypeColours[0] = 0x964b0000;
				TypeColours[1] = 0x96710000;
				TypeColours[2] = 0x96260000;

				PixelUpdateOrderCount = 0;
				InsertPixelUpdateOrder(0, std::vector<short>() = {});
		}

		bool isUpdateable() { return false; }
private:
};