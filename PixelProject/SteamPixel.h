#pragma once
#include "BasePixel.h"

class SteamPixel final : public BasePixel
{
public:

   SteamPixel()
   {
      pixel_type = E_PixelType::Steam;
      is_updateable = true;
      SetPixelName("Steam");

      colour_count = 2;
      // Browns
      type_colours[0] = 0xC7D5E0FF;
      type_colours[1] = 0xE6E6FAFF;

      pixel_update_order_count_ = 2;
      InsertPixelUpdateOrder(0, std::vector<short>() =
                             {North, NorthEast, NorthWest, South});
      InsertPixelUpdateOrder(1, std::vector<short>() =
                             {North, NorthWest, NorthEast, South});
   }

protected:
   int8_t UpdatePixel(const E_PixelType neighbour, E_PixelType pixel_results[2], int8_t direction) override
   {
      switch (direction)
      {
      case E_ChunkDirection::North:
      case E_ChunkDirection::NorthEast:
      case E_ChunkDirection::NorthWest:
      case E_ChunkDirection::South:
         return Logic(neighbour, pixel_results);
      default:
         return E_LogicResults::FailedUpdate;
      }
   }

private:
   inline int8_t Logic(const E_PixelType type, E_PixelType return_pixels[2])
   {
      int rngValue = rng() % 1000;
      switch (type)
      {
         case E_PixelType::Space:
         case E_PixelType::Steam:
         case E_PixelType::Water:
            if (rngValue <= 600)
            {
               return E_LogicResults::SuccessUpdate;
            }
            else if (rngValue <= 605)
            {
               return_pixels[0] = E_PixelType::Water;
               return E_LogicResults::FirstReturnPixel;
            }
         return E_LogicResults::FailedUpdate;
         default:
            if (rngValue <= 50)
            {
               return_pixels[0] = E_PixelType::Water;
               return E_LogicResults::FirstReturnPixel;
            }
      }
      return E_LogicResults::FailedUpdate;
   }
};
