#pragma once
#include "BasePixel.h"
#include "WorldDataHandler.h"

class WaterPixel : public BasePixel
{
public:
		virtual E_PixelType GetType() override { return E_PixelType::Water; }

		WaterPixel() {
				name = "Water";
				ColourCount = 3;
				// Browns
				TypeColours[0] = 0x0000FF00;
				TypeColours[1] = 0x2e2ee600;
				TypeColours[2] = 0x1818f300;
		}

		bool SE_Logic(E_PixelType type, E_PixelType returnPixels[2]) override { return Logic(type); }
		bool S_Logic(E_PixelType type, E_PixelType returnPixels[2]) override { return Logic(type); }
		bool SW_Logic(E_PixelType type, E_PixelType returnPixels[2]) override { return Logic(type); }
		bool W_Logic(E_PixelType type, E_PixelType returnPixels[2]) override { return Logic(type); }
		bool E_Logic(E_PixelType type, E_PixelType returnPixels[2]) override { return Logic(type); }

private:
		static bool Logic(E_PixelType type) {
				switch (type)
				{
				case E_PixelType::Space:
						return true;
				}
				return false;
		}

private:
};