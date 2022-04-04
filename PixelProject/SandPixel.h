#pragma once
#include "BasePixel.h"

class SandPixel final : public BasePixel
{
public:

   SandPixel()
   {
      pixel_type = E_PixelType::Sand;
      is_updateable = true;
      SetPixelName("Sand");

      colour_count = 3;
      // Yellows
      type_colours[0] = 0xE2D9AEFF;
      type_colours[1] = 0xE8BB9AFF;
      type_colours[2] = 0xEDC9AFFF;

      pixel_update_order_count_ = 2;
      InsertPixelUpdateOrder(0, std::vector<short>() =
                             {E_ChunkDirection::South, E_ChunkDirection::SouthWest, E_ChunkDirection::SouthEast});
      InsertPixelUpdateOrder(1, std::vector<short>() =
                             {E_ChunkDirection::South, E_ChunkDirection::SouthEast, E_ChunkDirection::SouthWest});
   }

protected:
   int8_t SouthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t SouthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
private:
   int8_t Logic(const E_PixelType type)
   {
      switch (type)
      {
      case E_PixelType::Space:
         return E_LogicResults::SuccessUpdate;
      case E_PixelType::Water:
         return (rng() % 3 == 0 ? E_LogicResults::SuccessUpdate : E_LogicResults::FailedUpdate);
      case E_PixelType::Oil:
         return (rng() % 10 == 0 ? E_LogicResults::SuccessUpdate : E_LogicResults::FailedUpdate);
      }
      return E_LogicResults::FailedUpdate;
   }
};
