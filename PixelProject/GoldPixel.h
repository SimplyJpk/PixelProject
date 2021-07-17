#pragma once
#include "BasePixel.h"

class GoldPixel : public BasePixel
{
public:
   E_PixelType GetType() override { return E_PixelType::Gold; }
   std::string Name() override { return "Gold"; }
   bool IsUpdateable() override { return true; }

   GoldPixel()
   {
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
   int8_t SouthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t SouthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }

private:
   inline int8_t Logic(const E_PixelType type)
   {
      switch (type)
      {
      case E_PixelType::Space:
      case E_PixelType::Water:
         return E_LogicResults::SuccessUpdate;
      case E_PixelType::Oil:
         return (rng() % 3 == 0 ? E_LogicResults::SuccessUpdate : E_LogicResults::FailedUpdate);
      case E_PixelType::Sand:
         return (rng() % 6 == 0 ? E_LogicResults::SuccessUpdate : E_LogicResults::FailedUpdate);
      }
      return E_LogicResults::FailedUpdate;
   }
};
