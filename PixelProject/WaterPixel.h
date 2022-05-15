#pragma once
#include "BasePixel.h"

class WaterPixel final : public BasePixel
{
public:

   WaterPixel()
   {
      pixel_type = E_PixelType::Water;
      is_updateable = true;
      SetPixelName("Water");

      MaxUpdateRange = 8;

      colour_count = 3;
      // Browns
      type_colours[0] = 0x0000FFFF;
      type_colours[1] = 0x2E2EE6FF;
      type_colours[2] = 0x1818F3FF;

      pixel_update_order_count_ = 2;
      InsertPixelUpdateOrder(0, std::vector<short>() =
                             {
                                E_ChunkDirection::SouthEast, E_ChunkDirection::SouthWest, E_ChunkDirection::South,
                                E_ChunkDirection::East, E_ChunkDirection::West
                             });
      InsertPixelUpdateOrder(1, std::vector<short>() =
                             {
                                E_ChunkDirection::SouthWest, E_ChunkDirection::SouthEast, E_ChunkDirection::South,
                                E_ChunkDirection::West, E_ChunkDirection::East
                             });
   }

   //x inline int8_t MaxUpdateRange() override { return 8; }

protected:
   void UpdatePixel(PixelUpdateResult& data) override
   {
      switch (data.Dir())
      {
      case E_ChunkDirection::East:
      case E_ChunkDirection::West:
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
      }
      data.Fail();
      return;
   }
};
