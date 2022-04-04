#pragma once
#include "BasePixel.h"

class SteamPixel final : public BasePixel
{
public:

   SteamPixel()
   {
      pixel_type = E_PixelType::Steam;
      is_updateable = true;
      SetPixelName("Steam");

      colour_count = 2;
      // Browns
      type_colours[0] = 0xC7D5E0FF;
      type_colours[1] = 0xE6E6FAFF;

      pixel_update_order_count_ = 2;
      InsertPixelUpdateOrder(0, std::vector<short>() =
                             {North, NorthEast, NorthWest, South});
      InsertPixelUpdateOrder(1, std::vector<short>() =
                             {North, NorthWest, NorthEast, South});
   }

protected:
   void UpdatePixel(PixelUpdateResult& data) override
   {
      switch (data.Dir())
      {
      case E_ChunkDirection::North:
      case E_ChunkDirection::NorthEast:
      case E_ChunkDirection::NorthWest:
      case E_ChunkDirection::South:
         Logic(data);
         return;
      default:
         data.Fail();
      }
   }

private:
   void Logic(PixelUpdateResult& data)
   {
      int rngValue = rng() % 1000;
      switch (data.NeighbourType())
      {
      case E_PixelType::Space:
      case E_PixelType::Steam:
      case E_PixelType::Water:
         if (rngValue <= 600)
         {
            data.Pass();
            return;
         }
         else if (rngValue <= 605)
         {
            data.SetLocal(E_PixelType::Water);
            return;
         }
         data.Fail();
         return;
      default:
         if (rngValue <= 50)
         {
            data.SetLocal(E_PixelType::Water);
            return;
         }
      }
      data.Fail();
   }
};
