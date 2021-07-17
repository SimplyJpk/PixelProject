#pragma once
#include <SDL_stdinc.h>

constexpr Uint32 pixel_index_bits            = 0b0000'0000'0000'0000'0000'0000'0001'1111;

constexpr Uint32 pixel_lifetime_bits         = 0b1111'1100'0000'0000'0000'0000'0000'0000;

constexpr Uint32 pixel_behaviour_bits        = 0b0000'0011'1100'0000'0000'0000'0000'0000;
