#pragma once

class WaterPixel : public BasePixel
{
public:
		WaterPixel() {
				ColourCount = 3;
				// Browns
				TypeColours[0] = 0x0000FF00;
				TypeColours[1] = 0x2e2ee600;
				TypeColours[2] = 0x1818f300;
		}

		const char* Name()
		{
				return "Water";
		}

private:
};