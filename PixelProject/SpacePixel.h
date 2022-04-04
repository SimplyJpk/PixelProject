#pragma once
#include "BasePixel.h"

class SpacePixel final : public BasePixel
{
public:

   SpacePixel()
   {
      pixel_type = E_PixelType::Space;
      is_updateable = false;
      SetPixelName("Space");

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

protected:
   int8_t UpdatePixel(const E_PixelType neighbour, E_PixelType pixel_results[2], int8_t direction) override
   {
      return E_LogicResults::FailedUpdate;
   }

private:
};
