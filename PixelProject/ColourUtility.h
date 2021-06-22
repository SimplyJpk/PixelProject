#pragma once

namespace PixelProject::Utility
{
   // Returns the colour from output
   inline void HexTo4F(const Uint32 hexValue, float* output)
   {
      output[0] = (hexValue >> 24 & 0xFF) / 255.0f; // RR
      output[1] = (hexValue >> 16 & 0xFF) / 255.0f; // GG
      output[2] = (hexValue >> 8 & 0xFF) / 255.0f; // BB
      output[3] = (hexValue & 0xFF) / 255.0f; // AA
   }

   // Returns the colour as a 4f*, must be deleted after use.
   inline float* HexTo4F(const Uint32 hexValue)
   {
      auto* output = new float[4];
      HexTo4F(hexValue, output);
      return output;
   }
}