#pragma once
#include "BasePixel.h"

class AcidPixel : public BasePixel
{
public:

   AcidPixel()
   {
      pixel_type = E_PixelType::Acid;
      is_updateable = true;
      SetPixelName("Acid");

      colour_count = 3;
      // Yellows
      type_colours[0] = 0xB0BF1AFF;
      type_colours[1] = 0xCBE315FF;
      type_colours[2] = 0x00FF00FF;

      pixel_update_order_count_ = 2;
      InsertPixelUpdateOrder(0, std::vector<short>() =
         { E_ChunkDirection::South, E_ChunkDirection::SouthWest, E_ChunkDirection::SouthEast, E_ChunkDirection::West, E_ChunkDirection::North });
      InsertPixelUpdateOrder(1, std::vector<short>() =
         { E_ChunkDirection::South, E_ChunkDirection::SouthEast, E_ChunkDirection::SouthWest, E_ChunkDirection::East });
   }

protected:
   int8_t NorthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override
   {
      if (type == E_PixelType::Water)
      {
         return_pixels[0] = E_PixelType::Water;
         return E_LogicResults::FirstReturnPixel;
      }
      return E_LogicResults::FailedUpdate;
   }
   int8_t WestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }
   int8_t EastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }
   int8_t SouthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }
   int8_t SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }
   int8_t SouthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }
private:
   inline int8_t Logic(const E_PixelType type, E_PixelType return_pixels[2])
   {
      switch (type)
      {
      case E_PixelType::Space:
         return E_LogicResults::SuccessUpdate;

         // 3 in 4 chance to move down in water, otherwise we convert the Acid into water
      case E_PixelType::Water:
         if (rng() % 10 != 0)
            return E_LogicResults::SuccessUpdate;
         else {
            return_pixels[0] = E_PixelType::Water;
            return E_LogicResults::FirstReturnPixel;
         }

      case E_PixelType::Acid:
         if (rng() % 6 == 0)
            return E_LogicResults::NoChange;
         return E_LogicResults::FailedUpdate;

      case E_PixelType::Ground:
         return E_LogicResults::FailedUpdate;

      default:
         int test = rng() % 5;
         if (test <= 1)
         {
            return_pixels[0] = E_PixelType::Space;
            return_pixels[1] = E_PixelType::Acid;
            return E_LogicResults::DualReturnPixel;
         }
         else if (test == 2)
         {
            return_pixels[0] = E_PixelType::Space;
            return_pixels[1] = E_PixelType::Space;
            return E_LogicResults::DualReturnPixel;
         }

         return E_LogicResults::FailedUpdate;
      }
   }
};
