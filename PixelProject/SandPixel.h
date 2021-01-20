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
      type_colours[0] = 0x00E2D9AE;
      type_colours[1] = 0x00E8BB9A;
      type_colours[2] = 0x00EDC9AF;

      pixel_update_order_count_ = 2;
      InsertPixelUpdateOrder(0, std::vector<short>() =
                             {E_ChunkDirection::South, E_ChunkDirection::SouthWest, E_ChunkDirection::SouthEast});
      InsertPixelUpdateOrder(1, std::vector<short>() =
                             {E_ChunkDirection::South, E_ChunkDirection::SouthEast, E_ChunkDirection::SouthWest});
   }

protected:
   int8_t SouthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t SouthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
private:
   inline int8_t Logic(const E_PixelType type)
   {
      switch (type)
      {
      case E_PixelType::Space:
         return E_LogicResults::SuccessUpdate;
      case E_PixelType::Water:
         return (pixel_rng_() % 3 == 0 ? E_LogicResults::SuccessUpdate : E_LogicResults::FailedUpdate);
      case E_PixelType::Oil:
         return (pixel_rng_() % 10 == 0 ? E_LogicResults::SuccessUpdate : E_LogicResults::FailedUpdate);
      }
      return E_LogicResults::FailedUpdate;
   }
};
