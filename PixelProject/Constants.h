#pragma once

namespace PixelProject::Constant
{
   constexpr short chunk_size_x = 128;
   constexpr short chunk_size_y = 128;
   constexpr short chunk_total_size = chunk_size_x * chunk_size_y;

   constexpr short world_size_x = 8;
   constexpr short world_size_y = 5;

   constexpr int8_t pixel_max_colour_count = 10;
   constexpr int8_t pixel_max_pixel_update_order = 4;
}

enum E_LogicResults : int8_t
{
   None = 0,
   FailedUpdate = 1 << 0,
   SuccessUpdate = 1 << 1,
   FirstReturnPixel = 1 << 2,
   SecondReturnPixel = 1 << 3,
   DualReturnPixel = 1 << 4
};