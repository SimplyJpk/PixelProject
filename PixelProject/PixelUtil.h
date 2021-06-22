#pragma once
#include <SDL.h>

namespace PixelProject::Utility
{
   inline Uint32 PixelIndexMask     = 0b00000000'00000000'00000000'00111111;
   inline Uint32 PixelVariantMask   = 0b00000000'00000000'00000001'11000000;

   inline short GetType(const Uint32 pixel)
   {
      return pixel & PixelIndexMask;
   }
   inline short GetVariant(const Uint32 pixel)
   {
      return pixel & PixelVariantMask;
   }

   inline Uint32 SetVariant(const Uint32 pixel, short data)
   {
      //TODO Continue from here
   }


}
