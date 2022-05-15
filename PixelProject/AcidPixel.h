#pragma once
#include "BasePixel.h"

class AcidPixel final : public BasePixel
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
   void UpdatePixel(PixelUpdateResult& data) override
   {
      switch (data.Dir())
      {
      case E_ChunkDirection::North:
         NorthLogic(data);
         return;
      case E_ChunkDirection::East:
      case E_ChunkDirection::West:
      case E_ChunkDirection::SouthEast:
      case E_ChunkDirection::South:
      case E_ChunkDirection::SouthWest:
         Logic(data);
         return;
      default:
         data.SetResult(E_LogicResults::FailedUpdate);
      }
   }

private:
   void NorthLogic(PixelUpdateResult& data)
   {
      if (data.NeighbourType() == E_PixelType::Water)
      {
         data.SetLocal(E_PixelType::Water);
         return;
      }
      data.SetResult(E_LogicResults::FailedUpdate);
   }

   void Logic(PixelUpdateResult& data)
   {
      switch (data.NeighbourType())
      {
      case E_PixelType::Space:
         data.SetResult(E_LogicResults::SuccessUpdate);
         return;

         // 3 in 4 chance to move down in water, otherwise we convert the Acid into water
      case E_PixelType::Water:
         if (rng() % 10 != 0) {
            data.SetResult(E_LogicResults::SuccessUpdate);
            return;
         }
         else {
            data.SetLocal(E_PixelType::Water);
            return;
         }

      case E_PixelType::Acid:
         if (rng() % 6 == 0) {
            data.SetResult(E_LogicResults::NoChange);
            return;
         }
         data.SetResult(E_LogicResults::FailedUpdate);
         return;

      case E_PixelType::Ground:
         data.SetResult(E_LogicResults::FailedUpdate);
         return;

      default:
         int test = rng() % 5;
         if (test <= 1)
         {
            data.SetLocalAndNeighbour(E_PixelType::Space, E_PixelType::Acid);
            return;
         }
         else if (test == 2)
         {
            data.SetLocalAndNeighbour(E_PixelType::Space, E_PixelType::Space);
            return;
         }

         data.SetResult(E_LogicResults::FailedUpdate);
         return;
      }
   }
};
