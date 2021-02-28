#pragma once
#include <SDL.h>
//#include "lib/XoshiroCpp.hpp"

#include "ChunkDirection.h"
#include "Constants.h"

using namespace PixelProject;

class BasePixel
{
public:
   virtual E_PixelType GetType() { return E_PixelType::UNDEFINED; }
   virtual const char* Name() { return name; }
   virtual bool IsUpdateable() { return true; }

   const char* name = "UNKNOWN";

   short* GetSingleChunkOrder()
   {
      if (pixel_update_order_count_ != 1)
      {
         chunk_order_counter_++;
         if (chunk_order_counter_ >= pixel_update_order_count_)
         {
            chunk_order_counter_ = 0;
         }
      }
      return pixel_update_order_[chunk_order_counter_];
   }

   short pixel_index = -1;
   short colour_count = 0;

   Uint32 GetRandomColour() { return type_colours[(colour_count <= 1 ? 0 : rand() % (colour_count - 1))]; }
   Uint32 type_colours[Constant::pixel_max_colour_count] = {0};

   virtual inline int8_t MaxUpdateRange() { return 1; }

   // Calls the derived North (UP) Pixel logic, returning a E_LogicResults value.
   virtual int8_t NorthLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; }
   // Calls the derived North-East (UP RIGHT) Pixel logic, returning a E_LogicResults value.
   virtual int8_t NorthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; }
   // Calls the derived East (RIGHT) Pixel logic, returning a E_LogicResults value.
   virtual int8_t EastLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; }
   // Calls the derived South-East (DOWN RIGHT) Pixel logic, returning a E_LogicResults value.
   virtual int8_t SouthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; }
   // Calls the derived South (DOWN) Pixel logic, returning a E_LogicResults value.
   virtual int8_t SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; }
   // Calls the derived South-West (DOWN RIGHT) Pixel logic, returning a E_LogicResults value.
   virtual int8_t SouthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; }
   // Calls the derived West (RIGHT) Pixel logic, returning a E_LogicResults value.
   virtual int8_t WestLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; }
   // Calls the derived North-West (UP RIGHT) Pixel logic, returning a E_LogicResults value.
   virtual int8_t NorthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) { return false; }

protected:
   ~BasePixel() = default;
   short chunk_order_counter_ = 0;
   // A very messy solution to help with pixel order processing
   short pixel_update_order_count_ = 1;
   short pixel_update_order_[Constant::pixel_max_pixel_update_order][E_ChunkDirection::DIR_COUNT] = {
      {
         E_ChunkDirection::South, E_ChunkDirection::SouthWest, E_ChunkDirection::SouthEast, E_ChunkDirection::East,
         E_ChunkDirection::West, E_ChunkDirection::NorthWest, E_ChunkDirection::NorthEast, E_ChunkDirection::North
      }
   };

   void InsertPixelUpdateOrder(const int index, std::vector<short> directions)
   {
      if (index < Constant::pixel_max_pixel_update_order)
      {
         for (short i = 0; i < E_ChunkDirection::DIR_COUNT; i++)
         {
            if (i < directions.size())
               pixel_update_order_[index][i] = directions[i];
            else
               pixel_update_order_[index][i] = E_ChunkDirection::DIR_COUNT;
         }
      }
   }

   //inline static std::uint64_t xor_shift_seed_ = 1;
   //inline static XoshiroCpp::Xoroshiro128PlusPlus pixel_rng_ { xor_shift_seed_ };
private:
};
