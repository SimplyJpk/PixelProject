#pragma once
#include <unordered_map>

#include "BasePixel.h"

//TODO Is this the best way? Couldn't I just json/xml something and read/load this all at runtime
#include "GroundPixel.h"
#include "WaterPixel.h"

//TODO Should this be a singleton? or a static class? The contained information shouldn't change.
class WorldDataHandler
{
public:
		const char* GetPixelName(Uint32 index) { return PixelTypes[index]->Name(); }
		const BasePixel* GetPixelType(Uint32 index) { return PixelTypes[index]; }

		WorldDataHandler() {
				BasePixel* pixels[] = { new GroundPixel(), new WaterPixel() };
				for (short index = 0; index < std::size(pixels); index++)
				{
						for (short i = 0; i < pixels[index]->ColourCount; i++)
						{
								if (PixelTypes.find(pixels[index]->TypeColours[i]) != PixelTypes.end()) {
										printf("WorldDataHandler already contains a Pixel of type '%s' with Colour Index: '%i'", pixels[index]->Name(), i);
								}
								PixelTypes.insert(std::make_pair(pixels[index]->TypeColours[i], pixels[index]));
						}
				}
		}
private:
		std::unordered_map<Uint32, BasePixel*> PixelTypes;
};

