#pragma once
#include <SDL.h>
#include <unordered_map>

#include "VecHash.h"

#include "WorldChunk.h"
#include "WorldRules.h"

#define MAX_PIXEL_COLOUR_COUNT 10

enum class E_PixelType {
		Space,
		Ground,
		Sand,
		Water
};

class BasePixel
{
public:
		virtual E_PixelType GetType() { return E_PixelType::Space; }
		virtual const char* Name() { return name; }
		virtual bool isUpdateable() { return true; }

		const char* name = "UNKNOWN";

		std::vector<short> GetSingleChunkOrder() { return ChunkDirectionOrder[rand() % ChunkDirectionOrder.size()]; }

		short PixelIndex = -1;
		short ColourCount = 0;

		bool UpdateLogic(const short dir, const E_PixelType type, E_PixelType returnPixels[2]) {
				switch (dir)
				{
				case North:
						return N_Logic(type, returnPixels);
				case NorthEast:
						return NE_Logic(type, returnPixels);
				case East:
						return E_Logic(type, returnPixels);
				case SouthEast:
						return SE_Logic(type, returnPixels);
				case South:
						return S_Logic(type, returnPixels);
				case SouthWest:
						return SW_Logic(type, returnPixels);
				case West:
						return W_Logic(type, returnPixels);
				case NorthWest:
						return NW_Logic(type, returnPixels);
				}
		}

		Uint32 GetRandomColour() { return TypeColours[(ColourCount <= 1 ? 0 : rand() % (ColourCount - 1))]; }
		Uint32 TypeColours[MAX_PIXEL_COLOUR_COUNT]{ 0 };
protected:
		virtual bool N_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool NE_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool E_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool SE_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool S_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool SW_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool W_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool NW_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };

		// A very messy solution to help with pixel order processing
		std::vector < std::vector<short>> ChunkDirectionOrder = {
		{ ChunkDirection::South, ChunkDirection::East, ChunkDirection::West, ChunkDirection::SouthWest, ChunkDirection::SouthEast, ChunkDirection::East, ChunkDirection::West, ChunkDirection::NorthWest, ChunkDirection::NorthEast, ChunkDirection::North }
		};
};



