#pragma once

namespace PixelProject::Utility
{
   // Returns the colour from output
   inline void HexTo4F(const Uint32 hex_value, float* output)
   {
      output[0] = (hex_value >> 24 & 0xFF) / 255.0f; // RR
      output[1] = (hex_value >> 16 & 0xFF) / 255.0f; // GG
      output[2] = (hex_value >> 8 & 0xFF) / 255.0f; // BB
      output[3] = (hex_value & 0xFF) / 255.0f; // AA
   }

   // Returns the colour as a 4f*, must be deleted after use.
   inline float* HexTo4F(const Uint32 hex_value)
   {
      auto* output = new float[4];
      HexTo4F(hex_value, output);
      return output;
   }
}