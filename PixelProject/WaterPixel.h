#pragma once
#include "BasePixel.h"
#include "WorldDataHandler.h"

class WaterPixel : public BasePixel
{
public:
		virtual const E_PixelType GetType() override { return E_PixelType::Water; }

		WaterPixel() {
				name = "Water";
				ColourCount = 3;
				// Browns
				TypeColours[0] = 0x0000FF00;
				TypeColours[1] = 0x2e2ee600;
				TypeColours[2] = 0x1818f300;

				PixelUpdateOrderCount = 2;
				InsertPixelUpdateOrder(0, std::vector<short>() =
						{ ChunkDirection::South, ChunkDirection::SouthEast, ChunkDirection::SouthWest, ChunkDirection::East, ChunkDirection::West });
				InsertPixelUpdateOrder(1, std::vector<short>() =
						{ ChunkDirection::South, ChunkDirection::SouthWest, ChunkDirection::SouthEast, ChunkDirection::West, ChunkDirection::East });
		}

		bool SE_Logic(const E_PixelType type, E_PixelType returnPixels[2]) override { return Logic(type); }
		bool S_Logic(const E_PixelType type, E_PixelType returnPixels[2]) override { return Logic(type); }
		bool SW_Logic(const E_PixelType type, E_PixelType returnPixels[2]) override { return Logic(type); }
		bool W_Logic(const E_PixelType type, E_PixelType returnPixels[2]) override { return Logic(type); }
		bool E_Logic(const E_PixelType type, E_PixelType returnPixels[2]) override { return Logic(type); }

private:
		const static bool Logic(E_PixelType type) {
				switch (type)
				{
				case E_PixelType::Space:
						return true;
				}
				return false;
		}

private:
};