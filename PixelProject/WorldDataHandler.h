#ifndef _WORLD_DATA_HANDLER_
#define _WORLD_DATA_HANDLER_
#include <unordered_map>

#include "BasePixel.h"

//TODO Should this be a singleton? or a static class? The contained information shouldn't change.
class WorldDataHandler
{
public:
		static WorldDataHandler* Instance() {
				if (!instance) {
						instance = new WorldDataHandler();
				}
				return instance;
		}

		// Information from Pixel Colours
		// Returns the name of a PixelType from a Pixel Colour
		const char* GetPixelName(Uint32 pixel) { return PixelColourMap[pixel]->Name(); }
		// Returns the BasePixel based on the pixel colour passed in
		BasePixel* GetPixelFromPixelColour(Uint32 pixel) { return PixelColourMap[pixel]; }

		// Returns base pixel from the Index provided, only use when calling using Pixel->PixelIndex
		BasePixel* GetPixelFromIndex(int index) { return PixelTypeList[index]; }

		// Information from PixelType (Enum)
		BasePixel* GetPixelFromType(E_PixelType type) { return PixelTypes[type]; }
		// Returns the name of a PixelType from the PixelType passed in
		const char* GetPixelName(E_PixelType type) { return PixelTypes[type]->Name(); }

		// Returns the number of main pixel types that exist in the game
		int PixelTypeCount() { return PixelTypeList.size(); }
		void FillWithPixelUpdateOrders(const short* output[]) {
				for (int i = 0; i < PixelTypeList.size(); i++) {
						output[i] = PixelTypeList[i]->GetSingleChunkOrder();
				}
		}


		void AddPixelData(BasePixel* pixel) {
				// Add pixels to an array we can access, and we set the pixels Index value
				pixel->PixelIndex = PixelTypeList.size();
				PixelTypeList.push_back(pixel);
				PixelTypes.insert(std::make_pair(pixel->GetType(), pixel));
				// Add our colours to our Lookup table
				for (short i = 0; i < pixel->ColourCount; i++)
				{
						if (PixelColourMap.find(pixel->TypeColours[i]) != PixelColourMap.end()) {
								printf("WorldDataHandler already contains a Pixel of type '%s' with Colour Index: '%i'", pixel->Name(), i);
						}
						PixelColourMap.insert(std::make_pair(pixel->TypeColours[i], pixel));
				}
		}

		WorldDataHandler() {};
		WorldDataHandler(WorldDataHandler const&) {};
		void operator=(WorldDataHandler const&) {};
private:
		static WorldDataHandler* instance;

		std::vector<BasePixel*> PixelTypeList;
		std::unordered_map<Uint32, BasePixel*> PixelColourMap;
		std::unordered_map<E_PixelType, BasePixel*> PixelTypes;
};
#endif