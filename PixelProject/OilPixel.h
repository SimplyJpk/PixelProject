#pragma once

#pragma once
#include "BasePixel.h"

class OilPixel final : public BasePixel
{
public:
   OilPixel()
   {
      pixel_type = E_PixelType::Oil;
      is_updateable = true;
      SetPixelName("Oil");

      colour_count = 3;
      // Browns
      type_colours[0] = 0x373A36FF;
      type_colours[1] = 0x232522FF;
      type_colours[2] = 0x5F615EFF;

      pixel_update_order_count_ = 2;
      InsertPixelUpdateOrder(0, std::vector<short>() =
                             {
                                E_ChunkDirection::North, E_ChunkDirection::NorthEast, E_ChunkDirection::NorthWest,
                                E_ChunkDirection::South, E_ChunkDirection::SouthEast, E_ChunkDirection::SouthWest,
                                E_ChunkDirection::East, E_ChunkDirection::West
                             });
      InsertPixelUpdateOrder(1, std::vector<short>() =
                             {
                                E_ChunkDirection::North, E_ChunkDirection::NorthWest, E_ChunkDirection::NorthEast,
                                E_ChunkDirection::South, E_ChunkDirection::SouthWest, E_ChunkDirection::SouthEast,
                                E_ChunkDirection::West, E_ChunkDirection::East
                             });
   }

protected:
   int8_t UpdatePixel(const E_PixelType neighbour, E_PixelType pixel_results[2], int8_t direction) override
   {
      switch (direction)
      {
      case E_ChunkDirection::North:
         return NorthLogic(neighbour);
      case E_ChunkDirection::NorthEast:
      case E_ChunkDirection::NorthWest:
         return NorthLogic(neighbour, 4);
      case E_ChunkDirection::SouthEast:
      case E_ChunkDirection::South:
      case E_ChunkDirection::SouthWest:
      case E_ChunkDirection::West:
      case E_ChunkDirection::East:
         return Logic(neighbour);
      default:
         return E_LogicResults::FailedUpdate;
      }
   }

private:
   inline int8_t NorthLogic(const E_PixelType type, const int odds = 2)
   {
      switch (type)
      {
      case E_PixelType::Water:
         return rng() % odds == 0 ? E_LogicResults::SuccessUpdate : E_LogicResults::FailedUpdate;
      }
      return E_LogicResults::FailedUpdate;
   }

   inline int8_t Logic(const E_PixelType type)
   {
      switch (type)
      {
      case E_PixelType::Space:
         return E_LogicResults::SuccessUpdate;
      }
      return E_LogicResults::FailedUpdate;
   }
};
