#pragma once
#include "Math.h"

namespace std
{
   template <>
   struct hash<IVec2>
   {
      size_t operator()(const IVec2& k) const noexcept
      {
         return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
      }

      bool operator()(const IVec2& a, const IVec2& b) const noexcept
      {
         return a.x == b.x && a.y == b.y;
      }
   };

   template <>
   struct hash<Vec2>
   {
      size_t operator()(const Vec2& k) const noexcept
      {
         return std::hash<float>()(k.x) ^ std::hash<float>()(k.y);
      }

      bool operator()(const Vec2& a, const Vec2& b) const noexcept
      {
         return a.x == b.x && a.y == b.y;
      }
   };
}
