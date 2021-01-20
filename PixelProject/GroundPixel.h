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
      type_colours[0] = 0x00964b00;
      type_colours[1] = 0x00967100;
      type_colours[2] = 0x00962600;

      pixel_update_order_count_ = 0;
      InsertPixelUpdateOrder(0, std::vector<short>() = {});
   }

   bool IsUpdateable() override { return false; }
private:
};
