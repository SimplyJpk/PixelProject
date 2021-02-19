#pragma once
#include "BasePixel.h"

class GroundPixel : public BasePixel
{
public:
   virtual E_PixelType GetType() override { return E_PixelType::Ground; }

   GroundPixel()
   {
      name = "Ground";
      colour_count = 3;
      // Browns
      type_colours[0] = 0x964b00FF;
      type_colours[1] = 0x967100FF;
      type_colours[2] = 0x962600FF;

      pixel_update_order_count_ = 0;
      InsertPixelUpdateOrder(0, std::vector<short>() = {});
   }

   bool IsUpdateable() override { return false; }
private:
};
