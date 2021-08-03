#pragma once
#include "BasePixel.h"

class SpacePixel : public BasePixel
{
public:
   E_PixelType GetType() override { return E_PixelType::Space; }
   std::string Name() override { return "Space"; }
   bool IsUpdateable() override { return false; }

   SpacePixel()
   {
      colour_count = 1;
      // Browns
      type_colours[0] = 0x00000000;

      pixel_update_order_count_ = 0;
      InsertPixelUpdateOrder(0, std::vector<short>() = {});
   }

   Uint32 GetNewPixel() override
   {
      return pixel_index;
   }

private:
};
