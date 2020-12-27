#pragma once
#include <SDL.h>
#include <functional>
#include <unordered_map>

#include "VecHash.h"

#include "WorldChunk.h"
#include "WorldRules.h"
#include "./lib/XoshiroCpp.hpp"
#include <time.h>

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
		virtual const E_PixelType GetType() { return E_PixelType::Space; }
		virtual const char* Name() { return name; }
		virtual bool isUpdateable() { return true; }

		const char* name = "UNKNOWN";

		short* GetSingleChunkOrder() {
				if (PixelUpdateOrderCount != 1) {
						_chunkOrderCounter++;
						if (_chunkOrderCounter >= PixelUpdateOrderCount) {
								_chunkOrderCounter = 0;
						}
				}
				return PixelUpdateOrder[_chunkOrderCounter];
		}

		short PixelIndex = -1;
		short ColourCount = 0;

		Uint32 GetRandomColour() { return TypeColours[(ColourCount <= 1 ? 0 : PixelRNG() % (ColourCount - 1))]; }
		Uint32 TypeColours[MAX_PIXEL_COLOUR_COUNT]{ 0 };

		virtual bool N_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool NE_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool E_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool SE_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool S_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool SW_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool W_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
		virtual bool NW_Logic(const E_PixelType type, E_PixelType returnPixels[2]) { return false; };
protected:
		short _chunkOrderCounter = 0;
		// A very messy solution to help with pixel order processing
		short PixelUpdateOrderCount = 1;
		short PixelUpdateOrder[MAX_PIXELUPDATE_ORDER_COUNT][ChunkDirection::DIR_COUNT] = { { ChunkDirection::South, ChunkDirection::SouthWest, ChunkDirection::SouthEast, ChunkDirection::East, ChunkDirection::West, ChunkDirection::NorthWest, ChunkDirection::NorthEast, ChunkDirection::North } };

		void InsertPixelUpdateOrder(int index, std::vector<short> directions) {
				if (index < MAX_PIXELUPDATE_ORDER_COUNT) {
						for (int i = 0; i < ChunkDirection::DIR_COUNT; i++) {
								if (i < directions.size())
										PixelUpdateOrder[index][i] = directions[i];
								else
										PixelUpdateOrder[index][i] = ChunkDirection::DIR_COUNT;
						}
				}
		}

		BasePixel() {};
private:
		XoshiroCpp::SplitMix64 PixelRNG{ time(NULL) };
};