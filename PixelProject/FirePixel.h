#pragma once
#include "BasePixel.h"

class FirePixel final : public BasePixel
{
public:
   short minLifetime = 5;
   uint8_t currentLifetime = PBit::LifetimeMaxValue();

   FirePixel()
   {
      pixel_type = E_PixelType::Fire;
      is_updateable = true;
      SetPixelName("Fire");

      colour_count = 3;
      // Browns
      type_colours[0] = 0xE25822FF;
      type_colours[1] = 0x800909FF;
      type_colours[2] = 0xD73502FF;

      pixel_update_order_count_ = 2;
      InsertPixelUpdateOrder(0, std::vector<short>() =
         { North, NorthEast, NorthWest, SouthEast, SouthWest, South });
      InsertPixelUpdateOrder(1, std::vector<short>() =
         { North, NorthWest, NorthEast, SouthWest, SouthEast, South });
   }

   Uint32 GetNewPixel() override
   {
      // We always want it to glow
      constexpr Uint32 lightLevel = 7 << PBit::LightDepth();

      Uint32 lifeTime = currentLifetime-- << PBit::LifetimeDepth();
      if (currentLifetime < minLifetime)
         currentLifetime = PBit::LifetimeMaxValue();

      return lightLevel | pixel_index | lifeTime;
   }

   bool PixelLifeTimeUpdate(Uint32& pixel, const uint8_t rng_value) override
   {
      //TODO Maybe Make a subtract 1 method for lifetime so we don't have to do this every frame

      // 1 in 5 chance to reduce lifetime.
      if (rng_value < 80)
         return true;

      auto lifetime = PBit::Lifetime(pixel);
      lifetime -= 1;

      PBit::SetLifetime(pixel, lifetime);

      return lifetime > 0;
   }

protected:
   void UpdatePixel(PixelUpdateResult& data) override
   {
      switch (data.Dir())
      {
      case E_ChunkDirection::North:
      case E_ChunkDirection::NorthEast:
      case E_ChunkDirection::NorthWest:
      case E_ChunkDirection::SouthEast:
      case E_ChunkDirection::South:
      case E_ChunkDirection::SouthWest:
         Logic(data);
         return;
      default:
         data.Fail();
      }
   }

private:
   void Logic(PixelUpdateResult& data)
   {
      switch (data.NeighbourType())
      {
      case E_PixelType::Space:
         rng() % 2 == 0 ? data.Pass() : data.Fail();
         return;


      case E_PixelType::Oil:
         data.SetLocalAndNeighbour(E_PixelType::Fire, E_PixelType::Fire);
         return;


      case E_PixelType::Wood:
         if (rng() % 10 == 0)
         {
            data.SetLocalAndNeighbour(E_PixelType::Fire, E_PixelType::Fire);
            return;
         }
         data.Fail();
         return;


      case E_PixelType::Fire:
         data.Fail();
         return;

      case E_PixelType::Water:
         data.SetLocalAndNeighbour(E_PixelType::Steam, E_PixelType::Space);
         return;
      }

      data.SetLocal(E_PixelType::Space);
      return;
   }
};
