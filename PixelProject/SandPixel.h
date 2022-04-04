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
         data.Pass();
         return;
      case E_PixelType::Water:
         (rng() % 3 == 0 ? data.Pass() : data.Fail());
         return;
      case E_PixelType::Oil:
         (rng() % 10 == 0 ? data.Pass() : data.Fail());
         return;
      }
      data.Fail();
   }
};
