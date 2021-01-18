#pragma once
/// <summary>
/// Container class that has 2 floats
/// </summary>
struct Vec2
{
public:
   float x = 0.0f;
   float y = 0.0f;

   Vec2(float x, float y)
   {
      this->x = x;
      this->y = y;
   }

   Vec2()
   {
   };

   Vec2 operator +(Vec2 other)
   {
      this->x + other.x;
      this->y + other.y;
   }

   Vec2 operator =(Vec2 other)
   {
      this->x = other.x;
      this->y = other.y;
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

/// <summary>
/// Container class that has 2 ints
/// </summary>
struct IVec2
{
public:
   int x = 0;
   int y = 0;

   IVec2()
   {
   };

   IVec2(int x, int y)
   {
      this->x = x;
      this->y = y;
   }

   IVec2 operator +(IVec2 other)
   {
      this->x + other.x;
      this->y + other.y;
   }

   IVec2 operator =(IVec2 other)
   {
      this->x = other.x;
      this->y = other.y;
      return *this;
   }

   bool operator==(const IVec2& other) const
   {
      return (this->x == other.x && this->y == other.y);
   }

   IVec2(const IVec2& rhs)
   {
      this->x = rhs.x;
      this->y = rhs.y;
   }

   static IVec2 Zero() { return IVec2(0, 0); };
   static IVec2 Left() { return IVec2(-1, 0); };
   static IVec2 Right() { return IVec2(1, 0); };
   static IVec2 Up() { return IVec2(0, 1); };
   static IVec2 Down() { return IVec2(0, -1); };

   template <class Archive>
   void serialize(Archive& archive)
   {
      archive(x, y);
   }
};
