#pragma once

#pragma once
#include "BasePixel.h"

class OilPixel : public BasePixel
{
public:
   virtual E_PixelType GetType() override { return E_PixelType::Oil; }

   OilPixel()
   {
      name = "Oil";
      colour_count = 3;
      // Browns
      type_colours[0] = 0x00373A36;
      type_colours[1] = 0x00232522;
      type_colours[2] = 0x005F615E;

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

   int8_t NorthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return NorthLogic(type, 4); }
   int8_t NorthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return NorthLogic(type, 4); }
   int8_t NorthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return NorthLogic(type); }

   int8_t SouthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t SouthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t WestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t EastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }

private:
   inline int8_t NorthLogic(const E_PixelType type, const int odds = 2)
   {
      switch (type)
      {
      case E_PixelType::Water:
         return rand() % odds == 0 ? E_LogicResults::SuccessUpdate : E_LogicResults::FailedUpdate;
      }
      return E_LogicResults::FailedUpdate;
   }

   inline int8_t Logic(const E_PixelType type)
   {
      switch (type)
      {
      case E_PixelType::Space:
         return E_LogicResults::SuccessUpdate;
      }
      return E_LogicResults::FailedUpdate;
   }

private:
};
