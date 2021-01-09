#pragma once
#include "BasePixel.h"

class WoodPixel : public BasePixel
{
public:
		virtual E_PixelType GetType() override { return E_PixelType::Wood; }

		WoodPixel() {
				name = "Wood";
				colour_count = 3;
				// Browns
				type_colours[0] = 0x42101000;
				type_colours[1] = 0x4e321100;
				type_colours[2] = 0x4e131100;

				pixel_update_order_count_ = 0;
				InsertPixelUpdateOrder(0, std::vector<short>() = {});
		}

		bool IsUpdateable() override { return false; }

private:
};