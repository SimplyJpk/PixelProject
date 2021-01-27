#pragma once

namespace PixelProject::Utility
{
   inline void bit_wise_toggle(int& input, const short n)
   {
      input ^= 1 << n;
   }

   inline void bit_wise_unset(int& input, const short n)
   {
      input &= ~(1 << n);
   }

   inline void bit_wise_set(int& input, const short n)
   {
      input |= 1 << n;
   }

   inline int bit_wise_get(const int input, const short n)
   {
      return (input >> n) & 1;
   }
}