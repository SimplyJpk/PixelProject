#pragma once
#include <SDL.h>

#include "ChunkSupport.h"
#include "Constants.h"
#include "XoshiroCpp.hpp"
#include <random>
#include "PixelDataMasks.h"

#include "ColourUtility.h"

using namespace PixelProject;
using namespace XoshiroCpp;

constexpr auto MAX_PIXEL_NAME_LENGTH = 10;

class BasePixel
{
public:
   E_PixelType pixel_type = E_PixelType::UNDEFINED;
   char pixel_name[MAX_PIXEL_NAME_LENGTH] = "None";
   
   bool is_updateable = false;
   bool ignore_pixel_update = true;

   inline static Xoshiro256PlusPlus rng;

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

   void GenerateColours()
   {
      for (int i = 0; i < colour_count; i++)
      {
         Utility::HexTo4F(type_colours[i], render_colours[i]);
      }
   }

   short pixel_index = -1;
   short colour_count = 0;

   Uint32 GetRandomColour() { return type_colours[(colour_count <= 1 ? 0 : rng() % (colour_count))]; }
   Uint32 type_colours[Constant::pixel_max_colour_count] = {0};

   float render_colours[Constant::pixel_max_colour_count][4] = { 0.0f };

   uint8_t MaxUpdateRange = 1;
   //x virtual inline int8_t MaxUpdateRange() { return 1; }

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

   // The Pixel updates itself without moving, pixels that use lifetimes may change their internal values.
   virtual bool PixelLifeTimeUpdate(Uint32& pixelValue, uint8_t rnd_value) { return true;}

   virtual Uint32 GetNewPixel()
   {
      return pixel_index;
   }

protected:
   virtual ~BasePixel() = default;

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

   void SetPixelName(const char* name)
   {
      if (strlen(name) > 0)
      {
         strcpy_s(pixel_name, MAX_PIXEL_NAME_LENGTH, name);
      }
   }
private:
};