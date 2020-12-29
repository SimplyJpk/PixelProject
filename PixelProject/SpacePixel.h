#pragma once
#include "BasePixel.h"
#include "WorldDataHandler.h"

class SpacePixel : public BasePixel
{
public:
		virtual E_PixelType GetType() override { return E_PixelType::Space; }

		SpacePixel() {
				name = "Space";
				colour_count = 1;
				// Browns
				type_colours[0] = 0x00000000;

				pixel_update_order_count_ = 0;
				InsertPixelUpdateOrder(0, std::vector<short>() = {});
		}

		bool IsUpdateable() { return false; }

private:
};