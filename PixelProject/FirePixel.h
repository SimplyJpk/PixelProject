#pragma once

#pragma once
#include "BasePixel.h"

class FirePixel : public BasePixel
{
public:
	 virtual E_PixelType GetType() override { return E_PixelType::Fire; }

	 FirePixel() {
			name = "Fire";
			colour_count = 3;
			// Browns
			type_colours[0] = 0xE2582200;
			type_colours[1] = 0x80090900;
			type_colours[2] = 0xD7350200;

			pixel_update_order_count_ = 2;
			InsertPixelUpdateOrder(0, std::vector<short>() =
				 { North, NorthEast, NorthWest, SouthEast, SouthWest, South });
			InsertPixelUpdateOrder(1, std::vector<short>() =
				 { North, NorthWest, NorthEast, SouthWest, SouthEast, South });
	 }

	 bool NorthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }
	 bool NorthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }
	 bool NorthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }

	 bool SouthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }
	 bool SouthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }
	 bool SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }

private:
	 inline bool Logic(const E_PixelType type, E_PixelType return_pixels[2]) {
			switch (type)
			{
			case E_PixelType::Space:
				 if (pixel_rng_() % 30 == 0)
				 {
						return_pixels[0] = E_PixelType::Space;
						return_pixels[1] = E_PixelType::Space;
				 }
				 return (pixel_rng_() % 2 == 0);

			case E_PixelType::Oil:
			   {
				 return_pixels[0] = E_PixelType::Fire;
				 return_pixels[1] = E_PixelType::Fire;
			   }
				 return true;

			case E_PixelType::Wood:
				 if (pixel_rng_() % 13 == 0)
				 {
						return_pixels[0] = E_PixelType::Fire;
						return_pixels[1] = E_PixelType::Fire;
						return true;
				 }

			case E_PixelType::Fire:
				 return false;
			}
			return_pixels[0] = E_PixelType::Space;
			return_pixels[1] = E_PixelType::UNDEFINED;
			return true;
	 }

private:
};