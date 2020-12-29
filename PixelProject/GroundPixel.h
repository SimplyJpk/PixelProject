#pragma once
#include "BasePixel.h"
#include "WorldDataHandler.h"

class GroundPixel : public BasePixel
{
public:
		virtual E_PixelType GetType() override { return E_PixelType::Ground; }

		GroundPixel() {
				name = "Ground";
				colour_count = 3;
				// Browns
				type_colours[0] = 0x964b0000;
				type_colours[1] = 0x96710000;
				type_colours[2] = 0x96260000;

				pixel_update_order_count_ = 0;
				InsertPixelUpdateOrder(0, std::vector<short>() = {});
		}

		bool IsUpdateable() { return false; }
private:
};