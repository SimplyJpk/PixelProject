#pragma once
#include "BasePixel.h"

class GoldPixel final : public BasePixel
{
public:

   GoldPixel()
   {
      pixel_type = E_PixelType::Gold;
      is_updateable = true;
      SetPixelName("Gold");

      colour_count = 2;
      // Browns
      type_colours[0] = 0xFFD700FF;
      type_colours[1] = 0xDAA520FF;

      pixel_update_order_count_ = 2;
      InsertPixelUpdateOrder(0, std::vector<short>() =
         { E_ChunkDirection::South, E_ChunkDirection::SouthWest, E_ChunkDirection::SouthEast });
      InsertPixelUpdateOrder(1, std::vector<short>() =
         { E_ChunkDirection::South, E_ChunkDirection::SouthEast, E_ChunkDirection::SouthWest });
   }

protected:
   int8_t UpdatePixel(const E_PixelType neighbour, E_PixelType pixel_results[2], int8_t direction) override
   {
      switch (direction)
      {
      case E_ChunkDirection::SouthEast:
      case E_ChunkDirection::South:
      case E_ChunkDirection::SouthWest:
         return Logic(neighbour);
      default:
         return false;
      }
   }

private:
   inline int8_t Logic(const E_PixelType type)
   {
      switch (type)
      {
      case E_PixelType::Space:
      case E_PixelType::Water:
         return E_LogicResults::SuccessUpdate;
      case E_PixelType::Oil:
         return (rng() % 3 == 0 ? E_LogicResults::SuccessUpdate : E_LogicResults::FailedUpdate);
      case E_PixelType::Sand:
         return (rng() % 6 == 0 ? E_LogicResults::SuccessUpdate : E_LogicResults::FailedUpdate);
      }
      return E_LogicResults::FailedUpdate;
   }
};
