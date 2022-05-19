#pragma once
#include "BasePixel.h"

class SourceBlock final : public BasePixel
{
public:

   E_PixelType spawned_type;
   SourceBlock(BasePixel* pixel)
   {
      pixel_type = GetSourceType(pixel->pixel_type);
      is_updateable = true;

      const std::string name = std::string(pixel->pixel_name) + "SRC";
      SetPixelName(name.c_str());

      colour_count = pixel->colour_count;
      type_colours[0] = pixel->type_colours[0];
      type_colours[1] = pixel->type_colours[1];
      type_colours[2] = pixel->type_colours[2];
      type_colours[3] = pixel->type_colours[3];

      spawned_type = pixel->pixel_type;

      pixel_update_order_count_ = 2;
      InsertPixelUpdateOrder(0, std::vector<short>() =
         { North, NorthEast, East, SouthEast, South, SouthWest, West, NorthWest });
      InsertPixelUpdateOrder(1, std::vector<short>() =
         { South, SouthEast, East, NorthEast, North, NorthWest, West, SouthWest });
   }

protected:
   void UpdatePixel(PixelUpdateResult& data) override
   {
      if (data.NeighbourType() == pixel_type)
      {
         data.Fail();
         return;
      }
      data.SetNewNeighbour(static_cast<E_PixelType>(spawned_type));
      return;
   }
private:

   E_PixelType GetSourceType(E_PixelType type)
   {
      switch (type)
      {
      case E_PixelType::Sand: return E_PixelType::SandSourceBlock;
      case E_PixelType::Water: return E_PixelType::WaterSourceBlock;
      case E_PixelType::Oil:  return E_PixelType::OilSourceBlock;
      case E_PixelType::Fire: return E_PixelType::FireSourceBlock;
      case E_PixelType::Acid: return E_PixelType::AcidSourceBlock;
      case E_PixelType::Gold: return E_PixelType::GoldSourceBlock;
      case E_PixelType::Steam: return E_PixelType::SteamSourceBlock;
      default: ;
      }
   }
};
