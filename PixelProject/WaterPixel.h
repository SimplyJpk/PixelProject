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
      type_colours[0] = 0x000000FF;
      type_colours[1] = 0x002E2EE6;
      type_colours[2] = 0x001818F3;

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

   int8_t SouthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t SouthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t WestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }
   int8_t EastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type); }

private:
   static int8_t Logic(const E_PixelType type)
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
