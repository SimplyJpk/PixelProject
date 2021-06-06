#pragma once
#include <cstdint>

namespace PixelProject::Constant
{
   // Chunk Size
   constexpr short chunk_size_x = 128;
   constexpr short chunk_size_y = 128;
   constexpr short chunk_total_size = chunk_size_x * chunk_size_y;

   // Corner Indexes of Chunks
   constexpr short chunk_n_w_corner_index = 0;
   constexpr short chunk_n_e_corner_index = chunk_size_x - 1;
   constexpr short chunk_s_w_corner_index = chunk_total_size - chunk_size_x;
   constexpr short chunk_s_e_corner_index = chunk_total_size - 1;

   // Number of chunks in the world
   constexpr short world_size_x = 8;
   constexpr short world_size_y = 5;

   // Max different colours that a PixelType can be
   constexpr int8_t pixel_max_colour_count = 10;
   // Max different pixel direction orders that a PixelType can have
   constexpr int8_t pixel_max_pixel_update_order = 4;

   //TODO Probably move this somewhere more sensible
   constexpr int16_t pixel_index_direction_change[] = {
      /* N   */   -Constant::chunk_size_x,
      /* NE  */   -Constant::chunk_size_x + 1,
      /* E   */   1,
      /* SE  */   Constant::chunk_size_x + 1,
      /* S   */   Constant::chunk_size_x,
      /* SW  */   Constant::chunk_size_x - 1,
      /* W   */   -1,
      /* NW  */   -Constant::chunk_size_x - 1
   };

   // A Bit index that represents that the new index will require moving to a new chunk.
   constexpr int8_t new_chunk_bit = 30;
   // A Bit index that represents the return number needs to be subtracted.
   constexpr int8_t negative_bit = 28;

   // North Border, Directional index value change.
   constexpr uint32_t pixel_index_north_border[] = {
      /* N   */   Constant::chunk_total_size - Constant::chunk_size_x | (1 << new_chunk_bit),
      /* NE  */   Constant::chunk_total_size - Constant::chunk_size_x + 1 | (1 << new_chunk_bit),
      /* E   */   1,
      /* SE  */   Constant::chunk_size_x + 1,
      /* S   */   Constant::chunk_size_x,
      /* SW  */   Constant::chunk_size_x - 1,
      /* W   */   1 | (1 << negative_bit),
      /* NW  */   Constant::chunk_total_size - Constant::chunk_size_x - 1 | (1 << new_chunk_bit)
   };
   // East Border, Directional index value change.
   constexpr uint32_t pixel_index_east_border[] = {
      /* N   */   Constant::chunk_size_x | (1 << negative_bit),
      /* NE  */   (Constant::chunk_size_x * 2) - 1 | (1 << new_chunk_bit) | (1 << negative_bit),
      /* E   */   Constant::chunk_size_x - 1 | (1 << new_chunk_bit) | (1 << negative_bit),
      /* SE  */   1 | (1 << new_chunk_bit),
      /* S   */   Constant::chunk_size_x,
      /* SW  */   Constant::chunk_size_x - 1,
      /* W   */   1 | (1 << negative_bit),
      /* NW  */   Constant::chunk_size_x + 1 | (1 << negative_bit)
   };
   // West Border, Directional index value change.
   constexpr uint32_t pixel_index_west_border[] = {
      /* N   */   Constant::chunk_size_x | (1 << negative_bit),
      /* NE  */   Constant::chunk_size_x - 1 | (1 << negative_bit),
      /* E   */   1,
      /* SE  */   Constant::chunk_size_x + 1,
      /* S   */   Constant::chunk_size_x,
      /* SW  */   (Constant::chunk_size_x * 2) - 1 | (1 << new_chunk_bit),
      /* W   */   Constant::chunk_size_x - 1 | (1 << new_chunk_bit),
      /* NW  */   1 | (1 << negative_bit) | (1 << new_chunk_bit)
   };
   // South Border, Directional index value change.
   constexpr uint32_t pixel_index_south_border[] = {
      /* N   */   Constant::chunk_size_x | (1 << negative_bit),
      /* NE  */   Constant::chunk_size_x - 1 | (1 << negative_bit),
      /* E   */   1,
      /* SE  */   Constant::chunk_total_size - Constant::chunk_size_x - 1 | (1 << negative_bit) | (1 << new_chunk_bit),
      /* S   */   Constant::chunk_total_size - Constant::chunk_size_x | (1 << negative_bit) | (1 << new_chunk_bit),
      /* SW  */   Constant::chunk_total_size - Constant::chunk_size_x + 1 | (1 << negative_bit) | (1 << new_chunk_bit),
      /* W   */   1 | (1 << negative_bit),
      /* NW  */   Constant::chunk_size_x + 1 | (1 << negative_bit)
   };
}

// Can be used to Identify the Index of a Pixel
enum class E_PixelType
{
   UNDEFINED = -1,
   Space,
   Ground,
   Sand,
   Water,
   Wood,
   Oil,
   Fire,
   Acid,
   Gold,
   Steam,
   COUNT
};

// Used to identify the results of Update Logic between Pixels.
enum E_LogicResults : int8_t
{
   None = 0,
   FailedUpdate = 1 << 0,
   SuccessUpdate = 1 << 1,
   FirstReturnPixel = 1 << 2,
   SecondReturnPixel = 1 << 3,
   DualReturnPixel = 1 << 4,
   NoChange = 1 << 5
};