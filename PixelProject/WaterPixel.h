#pragma once
#include "BasePixel.h"

class WaterPixel : public BasePixel
{
public:
   virtual E_PixelType GetType() override { return E_PixelType::Water; }

   WaterPixel()
   {
      name = "Water";
      colour_count = 3;
      // Browns
      type_colours[0] = 0x0000FF00;
      type_colours[1] = 0x2e2ee600;
      type_colours[2] = 0x1818f300;

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

   bool SouthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   bool SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   bool SouthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   bool WestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   bool EastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }

private:
   static bool Logic(const E_PixelType type)
   {
      switch (type)
      {
      case E_PixelType::Space:
         return true;
      }
      return false;
   }

private:
};
