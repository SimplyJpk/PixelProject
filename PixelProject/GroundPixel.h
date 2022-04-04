#pragma once
#include "BasePixel.h"

class GroundPixel final : public BasePixel
{
public:

   GroundPixel()
   {
      pixel_type = E_PixelType::Ground;
      is_updateable = false;
      SetPixelName("Ground");
      

      colour_count = 3;
      // Browns
      type_colours[0] = 0x964b00FF;
      type_colours[1] = 0x967100FF;
      type_colours[2] = 0x962600FF;

      pixel_update_order_count_ = 0;
      InsertPixelUpdateOrder(0, std::vector<short>() = {});
   }

   Uint32 GetNewPixel() override
   {
      constexpr Uint32 lightLevel = 5 << PBit::LightDepth();

      return lightLevel | pixel_index;
   }
private:
};
