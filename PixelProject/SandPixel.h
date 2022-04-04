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
   inline int8_t UpdatePixel(const E_PixelType neighbour, E_PixelType pixel_results[2], int8_t direction) override
   {
      switch (direction)
      {
      case E_ChunkDirection::SouthEast:
      case E_ChunkDirection::South:
      case E_ChunkDirection::SouthWest:
         return Logic(neighbour);
      default:
         return E_LogicResults::FailedUpdate;
      }
   }

private:
   inline int8_t Logic(const E_PixelType type)
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
