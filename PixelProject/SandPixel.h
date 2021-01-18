#pragma once
#include "BasePixel.h"

class SandPixel : public BasePixel
{
public:
   virtual E_PixelType GetType() override { return E_PixelType::Sand; }

   SandPixel()
   {
      name = "Sand";
      colour_count = 3;
      // Yellows
      type_colours[0] = 0xe2d9ae00;
      type_colours[1] = 0xe8bb9a00;
      type_colours[2] = 0xEDC9AF00;

      pixel_update_order_count_ = 2;
      InsertPixelUpdateOrder(0, std::vector<short>() =
                             {E_ChunkDirection::South, E_ChunkDirection::SouthWest, E_ChunkDirection::SouthEast});
      InsertPixelUpdateOrder(1, std::vector<short>() =
                             {E_ChunkDirection::South, E_ChunkDirection::SouthEast, E_ChunkDirection::SouthWest});
   }

protected:
   bool SouthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   bool SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   bool SouthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
private:
   inline bool Logic(const E_PixelType type)
   {
      switch (type)
      {
      case E_PixelType::Space:
         return true;
      case E_PixelType::Water:
         return (pixel_rng_() % 3 == 0);
      case E_PixelType::Oil:
         return (pixel_rng_() % 10 == 0);
      }
      return false;
   }
};
