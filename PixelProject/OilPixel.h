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
   void UpdatePixel(PixelUpdateResult& data) override
   {
      switch (data.Dir())
      {
         case E_ChunkDirection::North:
            NorthLogic(data, 2);
            return;
         case E_ChunkDirection::NorthEast:
         case E_ChunkDirection::NorthWest:
            NorthLogic(data, 4);
            return;
         case E_ChunkDirection::SouthEast:
         case E_ChunkDirection::South:
         case E_ChunkDirection::SouthWest:
         case E_ChunkDirection::West:
         case E_ChunkDirection::East:
            Logic(data);
            return;
      }
      data.Fail();
   }

private:
   void NorthLogic(PixelUpdateResult& data, int odds )
   {
      switch (data.NeighbourType())
      {
      case E_PixelType::Water:
         rng() % odds == 0 ? data.Pass() : data.Fail();
         return;
      }
      data.Fail();
   }

   void Logic(PixelUpdateResult& data)
   {
      switch (data.NeighbourType())
      {
      case E_PixelType::Space:
         data.Pass();
         return;
      }
      data.Fail();
   }
};
