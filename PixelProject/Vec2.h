#pragma once
/// <summary>
/// Container class that has 2 floats
/// </summary>
struct Vec2
{
public:
   float x = 0.0f;
   float y = 0.0f;

   Vec2(const float x, const float y)
   {
      this->x = x;
      this->y = y;
   }

   Vec2() = default;

   Vec2 operator +(const Vec2 other) const
   {
      auto result = Vec2(this->x + other.x, this->y + other.y);
      return result;
   }

   Vec2& operator =(const Vec2 other)
   {
      this->x = other.x;
      this->y = other.y;
      return *this;
   }

   static Vec2 Zero() { return Vec2(0, 0); };
   static Vec2 Left() { return Vec2(-1, 0); };
   static Vec2 Right() { return Vec2(1, 0); };
   static Vec2 Up() { return Vec2(0, 1); };
   static Vec2 Down() { return Vec2(0, -1); };

   template <class Archive>
   void serialize(Archive& archive)
   {
      archive(x, y);
   }
};