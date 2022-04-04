#pragma once
#include "BasePixel.h"

class WoodPixel final : public BasePixel
{
public:

   WoodPixel()
   {
      pixel_type = E_PixelType::Wood;
      is_updateable = false;
      SetPixelName("Wood");

      colour_count = 3;
      // Browns
      type_colours[0] = 0x421010FF;
      type_colours[1] = 0x4E3211FF;
      type_colours[2] = 0x4E1311FF;

      pixel_update_order_count_ = 0;
      InsertPixelUpdateOrder(0, std::vector<short>() = {});
   }

private:
};
