#pragma once
#include "BasePixel.h"

class WoodPixel : public BasePixel
{
public:
   virtual E_PixelType GetType() override { return E_PixelType::Wood; }

   WoodPixel()
   {
      name = "Wood";
      colour_count = 3;
      // Browns
      type_colours[0] = 0x00421010;
      type_colours[1] = 0x004E3211;
      type_colours[2] = 0x004E1311;

      pixel_update_order_count_ = 0;
      InsertPixelUpdateOrder(0, std::vector<short>() = {});
   }

   bool IsUpdateable() override { return false; }

private:
};
