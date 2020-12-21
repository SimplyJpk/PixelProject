#ifndef _WORLD_DATA_HANDLER_
#define _WORLD_DATA_HANDLER_
#include <unordered_map>

#include "BasePixel.h"

//TODO Is this the best way? Couldn't I just json/xml something and read/load this all at runtime
#include "GroundPixel.h"
#include "WaterPixel.h"
#include "SandPixel.h"
#include "SpacePixel.h"

//TODO Should this be a singleton? or a static class? The contained information shouldn't change.
class WorldDataHandler
{
public:
		static WorldDataHandler* Instance() {
				if (!instance) {
						instance = new WorldDataHandler();
						instance->ConstructData();
				}
				return instance;
		}

		const char* GetPixelName(Uint32 index) { return PixelTypes[index]->Name(); }
		BasePixel* GetPixelType(Uint32 index) { return PixelTypes[index]; }
		BasePixel* GetPixelFromIndex(int index) { return PixelTypeList[index]; }
		// Returns the number of main pixel types that exist in the game
		int PixelTypeCount() { return PixelTypeList.size(); }

		WorldDataHandler() {};
		WorldDataHandler(WorldDataHandler const&) {};
		void operator=(WorldDataHandler const&) {};
private:
		static WorldDataHandler* instance;

		std::vector<BasePixel*> PixelTypeList;
		std::unordered_map<Uint32, BasePixel*> PixelTypes;

		void ConstructData() {
				instance = this;
				BasePixel* pixels[] = { new SpacePixel(), new GroundPixel(), new WaterPixel(), new SandPixel() };
				for (short index = 0; index < std::size(pixels); index++)
				{
						// Add pixels to an array we can access, and we set the pixels Index value
						PixelTypeList.push_back(pixels[index]);
						pixels[index]->PixelIndex = index;
						// Add our colours to our Lookup table
						for (short i = 0; i < pixels[index]->ColourCount; i++)
						{
								if (PixelTypes.find(pixels[index]->TypeColours[i]) != PixelTypes.end()) {
										printf("WorldDataHandler already contains a Pixel of type '%s' with Colour Index: '%i'", pixels[index]->Name(), i);
								}
								PixelTypes.insert(std::make_pair(pixels[index]->TypeColours[i], pixels[index]));
						}
				}
		}
};
#endif