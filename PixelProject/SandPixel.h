#pragma once
#include "BasePixel.h"
#include "WorldDataHandler.h"

class SandPixel : public BasePixel
{
public:
		virtual const E_PixelType GetType() override { return E_PixelType::Sand; }

		SandPixel() {
				name = "Sand";
				ColourCount = 3;
				// Yellows
				TypeColours[0] = 0xe2d9ae00;
				TypeColours[1] = 0xe8bb9a00;
				TypeColours[2] = 0xEDC9AF00;

				PixelUpdateOrderCount = 2;
				InsertPixelUpdateOrder(0, std::vector<short>() =
						{ ChunkDirection::South, ChunkDirection::SouthWest, ChunkDirection::SouthEast });
				InsertPixelUpdateOrder(1, std::vector<short>() =
						{ ChunkDirection::South, ChunkDirection::SouthEast, ChunkDirection::SouthWest });
		}
		
protected:
		bool SE_Logic(const E_PixelType type, E_PixelType returnPixels[2]) override { return Logic(type); }
		bool S_Logic(const E_PixelType type, E_PixelType returnPixels[2]) override { return Logic(type); }
		bool SW_Logic(const E_PixelType type, E_PixelType returnPixels[2]) override { return Logic(type); }
private:
		static bool Logic(E_PixelType type) {
				switch (type)
				{
				case E_PixelType::Space:
						return true;
				case E_PixelType::Water:
						return (rand() % 3 == 0);
				}
				return false;
		}
};
