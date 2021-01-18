#pragma once
#include <SDL.h>
#include <functional>
#include <unordered_map>

#include "VecHash.h"

#include "WorldChunk.h"
#include "WorldRules.h"
#include "./lib/XoshiroCpp.hpp"
#include <ctime>

#define MAX_PIXEL_COLOUR_COUNT 10
#define MAX_PIXELUPDATE_ORDER_COUNT 4

enum class E_PixelType {
	 UNDEFINED = -1,
	 Space,
	 Ground,
	 Sand,
	 Water,
	 Wood,
	 Oil,
	 Fire,
	 COUNT
};

class BasePixel
{
public:
		virtual E_PixelType GetType() { return E_PixelType::UNDEFINED; }
		virtual const char* Name() { return name; }
		virtual bool IsUpdateable() { return true; }

		const char* name = "UNKNOWN";

		short* GetSingleChunkOrder() {
				if (pixel_update_order_count_ != 1) {
						chunk_order_counter_++;
						if (chunk_order_counter_ >= pixel_update_order_count_) {
								chunk_order_counter_ = 0;
						}
				}
				return pixel_update_order_[chunk_order_counter_];
		}

		short pixel_index = -1;
		short colour_count = 0;

		Uint32 GetRandomColour() { return type_colours[(colour_count <= 1 ? 0 : pixel_rng_() % (colour_count - 1))]; }
		Uint32 type_colours[MAX_PIXEL_COLOUR_COUNT] = { 0 };

		virtual bool NorthLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; };
		virtual bool NorthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; };
		virtual bool EastLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; };
		virtual bool SouthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; };
		virtual bool SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; };
		virtual bool SouthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; };
		virtual bool WestLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; };
		virtual bool NorthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; };
protected:
		~BasePixel() = default;
		short chunk_order_counter_ = 0;
		// A very messy solution to help with pixel order processing
		short pixel_update_order_count_ = 1;
		short pixel_update_order_[MAX_PIXELUPDATE_ORDER_COUNT][E_ChunkDirection::DIR_COUNT] = { { E_ChunkDirection::South, E_ChunkDirection::SouthWest, E_ChunkDirection::SouthEast, E_ChunkDirection::East, E_ChunkDirection::West, E_ChunkDirection::NorthWest, E_ChunkDirection::NorthEast, E_ChunkDirection::North } };

		void InsertPixelUpdateOrder(const int index, std::vector<short> directions) {
				if (index < MAX_PIXELUPDATE_ORDER_COUNT) {
						for (int i = 0; i < E_ChunkDirection::DIR_COUNT; i++) {
								if (i < directions.size())
										pixel_update_order_[index][i] = directions[i];
								else
										pixel_update_order_[index][i] = E_ChunkDirection::DIR_COUNT;
						}
				}
		}

		BasePixel() = default;

		XoshiroCpp::SplitMix64 pixel_rng_{ time(nullptr) };
private:
};