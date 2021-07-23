#pragma once
#include <SDL_stdinc.h>

//TODO What is the point of this if I have most of them repeated below? Heckin bits
namespace PMaskData
{
   /// <summary> Range: 0 - 31 [5 Bits]</summary>
   constexpr static Uint32 index_bits = 0b0000'0000'0000'0000'0000'0000'0001'1111;
   constexpr static uint8_t index_count = 5;
   constexpr static uint8_t index_depth = 0 + 1;

   /// <summary> Range: 0 - 63 [6 Bits]</summary>
   constexpr static Uint32 lifetime_bits = 0b1111'1100'0000'0000'0000'0000'0000'0000;
   constexpr static uint8_t lifetime_count = 6;
   constexpr static uint8_t lifetime_depth = 26 + 1;

   /// <summary> Range: 0 - 15 [4 Bits]</summary>
   constexpr static Uint32 behaviour_bits = 0b0000'0011'1100'0000'0000'0000'0000'0000;
   constexpr static uint8_t behaviour_count = 4;
   constexpr static uint8_t behaviour_depth = 22 + 1;

   /// <summary> Range: 0 - 8 [3 Bits]</summary>
   constexpr static Uint32 light_bits = 0b0000'0000'0000'0000'0000'0000'1110'0000;
   constexpr static uint8_t light_count = 3;
   constexpr static uint8_t light_depth = 5 + 1;
}


constexpr static Uint32 GetPixelIndexBits() { return PMaskData::index_bits; }
constexpr static Uint32 GetPixelIndexDepth() { return PMaskData::index_depth; }
constexpr static uint8_t GetP_Index(const Uint32 pixel)
{
   return (((1 << PMaskData::index_count) - 1) & ((pixel & PMaskData::index_bits) >> (PMaskData::index_depth - 1)));
}

constexpr static Uint32 GetPixelLifetimeDepth() { return PMaskData::lifetime_depth; }
constexpr static Uint32 GetPixelLifetimeBits() { return PMaskData::lifetime_bits; }
constexpr static uint8_t GetP_Lifetime(const Uint32 pixel)
{
   return (((1 << PMaskData::lifetime_count) - 1) & ((pixel & PMaskData::lifetime_bits) >> (PMaskData::lifetime_depth - 1)));
}

constexpr static Uint32 GetPixelBehaviourDepth() { return PMaskData::behaviour_depth; }
constexpr static Uint32 GetPixelBehaviourBits() { return PMaskData::behaviour_bits; }
constexpr static uint8_t GetP_BehaviourBits(const Uint32 pixel)
{
   return (((1 << PMaskData::behaviour_count) - 1) & ((pixel & PMaskData::behaviour_bits) >> (PMaskData::behaviour_depth - 1)));
}


constexpr static Uint32 GetPixelLightDepth() { return PMaskData::light_depth; }
constexpr static Uint32 GetPixelLightBits() { return PMaskData::light_bits; }
constexpr static uint8_t GetP_LightBits(const Uint32 pixel)
{
   return (((1 << PMaskData::light_count) - 1) & ((pixel & PMaskData::light_bits) >> (PMaskData::light_depth - 1)));
}
