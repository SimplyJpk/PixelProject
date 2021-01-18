#pragma once

#pragma once
#include "BasePixel.h"

class OilPixel : public BasePixel
{
public:
	 virtual E_PixelType GetType() override { return E_PixelType::Oil; }

	 OilPixel() {
			name = "Oil";
			colour_count = 3;
			// Browns
			type_colours[0] = 0x373A3600;
			type_colours[1] = 0x23252200;
			type_colours[2] = 0x5F615E00;

			pixel_update_order_count_ = 2;
			InsertPixelUpdateOrder(0, std::vector<short>() =
				 { E_ChunkDirection::North, E_ChunkDirection::NorthEast, E_ChunkDirection::NorthWest, E_ChunkDirection::South, E_ChunkDirection::SouthEast, E_ChunkDirection::SouthWest, E_ChunkDirection::East, E_ChunkDirection::West });
			InsertPixelUpdateOrder(1, std::vector<short>() =
				 { E_ChunkDirection::North, E_ChunkDirection::NorthWest, E_ChunkDirection::NorthEast, E_ChunkDirection::South, E_ChunkDirection::SouthWest, E_ChunkDirection::SouthEast, E_ChunkDirection::West, E_ChunkDirection::East });
	 }

	 bool NorthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return NorthLogic(type, 4); }
	 bool NorthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return NorthLogic(type, 4); }
	 bool NorthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return NorthLogic(type); }

	 bool SouthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
	 bool SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
	 bool SouthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
	 bool WestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
	 bool EastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }

private:
	 inline bool NorthLogic(const E_PixelType type, const int odds = 2)
	 {
			switch (type)
			{
			case E_PixelType::Water:
				 return (pixel_rng_() % odds == 0);
			}
			return false;
	 }

	 inline bool Logic(const E_PixelType type) {
			switch (type)
			{
			case E_PixelType::Space:
				 return true;
			}
			return false;
	 }

private:
};