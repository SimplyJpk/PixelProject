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
   void UpdatePixel(PixelUpdateResult& data) override
   {
      switch (data.Dir())
      {
      case E_ChunkDirection::SouthEast:
      case E_ChunkDirection::South:
      case E_ChunkDirection::SouthWest:
         Logic(data);
         return;
      default:
         data.Fail();
      }
   }

private:
   void Logic(PixelUpdateResult& data)
   {
      switch (data.NeighbourType())
      {
      case E_PixelType::Space:
      case E_PixelType::Water:
         data.Pass();
         return;
      case E_PixelType::Oil:
         rng() % 3 == 0 ? data.Pass() : data.Fail();
         return;
      case E_PixelType::Sand:
         rng() % 6 == 0 ? data.Pass() : data.Fail();
         return;
      default:
         data.Fail();
         return;
      }
   }
};
