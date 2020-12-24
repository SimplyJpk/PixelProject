#pragma once
#include <SDL.h>
#include <functional>
#include <unordered_map>

#include "VecHash.h"

#include "WorldChunk.h"
#include "WorldRules.h"

#define MAX_PIXEL_COLOUR_COUNT 10
#define MAX_PIXELUPDATE_ORDER_COUNT 4

enum class E_PixelType {
		Space,
		Ground,
		Sand,
		Water,
		COUNT
};

class BasePixel
{
public:
		virtual E_PixelType GetType() { return E_PixelType::Space; }
		virtual const char* Name() { return name; }
		virtual bool isUpdateable() { return true; }

		const char* name = "UNKNOWN";

		const short* GetSingleChunkOrder() {
				_chunkOrderCounter++;
				if (_chunkOrderCounter >= PixelUpdateOrderCount) {
						_chunkOrderCounter = 0;
				}
				return PixelUpdateOrder[_chunkOrderCounter];
		}

		short PixelIndex = -1;
		short ColourCount = 0;

		//TODO Can we remove this yet? Please?
		//? bool UpdateLogic(const short dir, const E_PixelType type, E_PixelType returnPixels[2]) {
		//? 		switch (dir)
		//? 		{
		//? 		case North:
		//? 				return N_Logic(type, returnPixels);
		//? 		case NorthEast:
		//? 				return NE_Logic(type, returnPixels);
		//? 		case East:
		//? 				return E_Logic(type, returnPixels);
		//? 		case SouthEast:
		//? 				return SE_Logic(type, returnPixels);
		//? 		case South:
		//? 				return S_Logic(type, returnPixels);
		//? 		case SouthWest:
		//? 				return SW_Logic(type, returnPixels);
		//? 		case West:
		//? 				return W_Logic(type, returnPixels);
		//? 		case NorthWest:
		//? 				return NW_Logic(type, returnPixels);
		//? 		}
		//? }

		Uint32 GetRandomColour() { return TypeColours[(ColourCount <= 1 ? 0 : rand() % (ColourCount - 1))]; }
		Uint32 TypeColours[MAX_PIXEL_COLOUR_COUNT]{ 0 };

		virtual bool N_Logic(E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool NE_Logic(E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool E_Logic(E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool SE_Logic(E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool S_Logic(E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool SW_Logic(E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool W_Logic(E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool NW_Logic(E_PixelType type, E_PixelType returnPixels[2]) { return false; };
protected:
		short _chunkOrderCounter = 0;
		// A very messy solution to help with pixel order processing
		short PixelUpdateOrderCount = 1;
		short PixelUpdateOrder[MAX_PIXELUPDATE_ORDER_COUNT][8] = { { ChunkDirection::South, ChunkDirection::SouthWest, ChunkDirection::SouthEast, ChunkDirection::East, ChunkDirection::West, ChunkDirection::NorthWest, ChunkDirection::NorthEast, ChunkDirection::North } };

		BasePixel() {};
};