#pragma once
#include "BasePixel.h"

class SteamPixel : public BasePixel
{
public:
   virtual E_PixelType GetType() override { return E_PixelType::Steam; }

   SteamPixel()
   {
      name = "Steam";
      colour_count = 2;
      // Browns
      type_colours[0] = 0xC7D5E0FF;
      type_colours[1] = 0xE6E6FAFF;

      pixel_update_order_count_ = 2;
      InsertPixelUpdateOrder(0, std::vector<short>() =
                             {North, NorthEast, NorthWest, South});
      InsertPixelUpdateOrder(1, std::vector<short>() =
                             {North, NorthWest, NorthEast, South});
   }

   int8_t NorthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override
   {
      return Logic(type, return_pixels);
   }

   int8_t NorthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override
   {
      return Logic(type, return_pixels);
   }

   int8_t NorthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }

   int8_t SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }

private:
   inline int8_t Logic(const E_PixelType type, E_PixelType return_pixels[2])
   {
      int rngValue = rng() % 1000;
      switch (type)
      {
         case E_PixelType::Space:
         case E_PixelType::Steam:
         case E_PixelType::Water:
            if (rngValue <= 600)
            {
               return E_LogicResults::SuccessUpdate;
            }
            else if (rngValue <= 605)
            {
               return_pixels[0] = E_PixelType::Water;
               return E_LogicResults::FirstReturnPixel;
            }
         return E_LogicResults::FailedUpdate;
         default:
            if (rngValue <= 50)
            {
               return_pixels[0] = E_PixelType::Water;
               return E_LogicResults::FirstReturnPixel;
            }
      }
      return E_LogicResults::FailedUpdate;
   }

private:
};
