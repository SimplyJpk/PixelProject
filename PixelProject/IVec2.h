#pragma once
#include <cmath>
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
   }

   IVec2(const int x, const int y)
   {
      this->x = x;
      this->y = y;
   }

   template <class Archive>
   void serialize(Archive& archive)
   {
      archive(x, y);
   }

   IVec2 operator +(const IVec2& other) const;
   IVec2 operator -(const IVec2& other) const;
   IVec2& operator =(const IVec2& other);
   IVec2 operator *(const float rhs) const;

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

   static IVec2 Lerp(IVec2 a, IVec2 b, float t);
   static float Distance(const IVec2& a, const IVec2& b);
};

inline IVec2& IVec2::operator=(const IVec2& other)
{
   this->x = other.x;
   this->y = other.y;
   return *this;
}

inline IVec2 IVec2::operator*(const float rhs) const
{
   return *(new IVec2(static_cast<int>(this->x * rhs), static_cast<int>(this->y * rhs)));
}

inline IVec2 IVec2::Lerp(IVec2 a, IVec2 b, float t)
{
   IVec2 result = a + (b - a) * t;
   return result;
}

inline float IVec2::Distance(const IVec2& a, const IVec2& b)
{
   const auto result = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
   return static_cast<float>(result);
}

inline IVec2 IVec2::operator+(const IVec2& other) const
{
   return *(new IVec2(this->x + other.x, this->y + other.y));
}

inline IVec2 IVec2::operator-(const IVec2& other) const
{
   return *(new IVec2(this->x - other.x, this->y - other.y));
}