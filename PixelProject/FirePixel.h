#pragma once
#include "BasePixel.h"

class FirePixel : public BasePixel
{
public:
   E_PixelType GetType() override { return E_PixelType::Fire; }
   std::string Name() override { return "Fire"; }
   bool IsUpdateable() override { return true; }

   FirePixel()
   {
      colour_count = 3;
      // Browns
      type_colours[0] = 0xE25822FF;
      type_colours[1] = 0x800909FF;
      type_colours[2] = 0xD73502FF;

      pixel_update_order_count_ = 2;
      InsertPixelUpdateOrder(0, std::vector<short>() =
                             {North, NorthEast, NorthWest, SouthEast, SouthWest, South});
      InsertPixelUpdateOrder(1, std::vector<short>() =
                             {North, NorthWest, NorthEast, SouthWest, SouthEast, South});
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

   int8_t SouthWestLogic(const E_PixelType type, E_PixelType return_pixels[2]) override
   {
      return Logic(type, return_pixels);
   }

   int8_t SouthEastLogic(const E_PixelType type, E_PixelType return_pixels[2]) override
   {
      return Logic(type, return_pixels);
   }

   int8_t SouthLogic(const E_PixelType type, E_PixelType return_pixels[2]) override { return Logic(type, return_pixels); }

   Uint32 GetNewPixel() override
   {
      // We always want it to glow
      constexpr Uint32 lightLevel = 7 << PBit::LightDepth();

      return lightLevel | pixel_index;
   }

private:
   inline int8_t Logic(const E_PixelType type, E_PixelType return_pixels[2])
   {
      switch (type)
      {
      case E_PixelType::Space:
         if (rng() % 30 == 0)
         {
            return_pixels[0] = E_PixelType::Space;
            return_pixels[1] = E_PixelType::Space;
            return E_LogicResults::DualReturnPixel;
         }
         return rng() % 2 == 0 ? E_LogicResults::SuccessUpdate : E_LogicResults::FailedUpdate;


      case E_PixelType::Oil:
         return_pixels[0] = E_PixelType::Fire;
         return_pixels[1] = E_PixelType::Fire;
         return E_LogicResults::DualReturnPixel;


      case E_PixelType::Wood:
         if (rng() % 13 == 0)
         {
            return_pixels[0] = E_PixelType::Fire;
            return_pixels[1] = E_PixelType::Fire;
            return E_LogicResults::DualReturnPixel;
         }
         return E_LogicResults::FailedUpdate;


      case E_PixelType::Fire:
         return E_LogicResults::FailedUpdate;
      }

      return_pixels[0] = E_PixelType::Space;
      return E_LogicResults::FirstReturnPixel;
   }

private:
};
