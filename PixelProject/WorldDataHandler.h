#pragma once
#include <unordered_map>

#include "BasePixel.h"
#include <array>

//TODO Should this be a singleton? or a static class? The contained information shouldn't change.
class WorldDataHandler
{
public:
   static WorldDataHandler* Instance()
   {
      if (!instance_)
      {
         instance_ = new WorldDataHandler();
      }
      return instance_;
   }

   // Information from Pixel Colours
   // Returns the name of a PixelType from a Pixel Colour
   const char* GetPixelName(const Uint32 pixel) { return pixel_colour_map_[pixel]->Name(); }
   // Returns the BasePixel based on the pixel colour passed in
   inline BasePixel* GetPixelFromPixelColour(const Uint32 pixel) { return pixel_colour_map_[pixel]; }

   // Returns base pixel from the Index provided, only use when calling using Pixel->PixelIndex
   BasePixel* GetPixelFromIndex(const short index) { return pixel_type_list_[index]; }

   // Information from PixelType (Enum)
   BasePixel* GetPixelFromType(const E_PixelType type) { return pixel_types_[type]; }
   // Returns the name of a PixelType from the PixelType passed in
   const char* GetPixelName(const E_PixelType type) { return pixel_types_[type]->Name(); }

   // Returns the number of main pixel types that exist in the game
   int PixelTypeCount() const { return pixel_type_counter_; }

   void FillWithPixelUpdateOrders(short* output[])
   {
      for (int i = 0; i < pixel_type_counter_; i++)
      {
         output[i] = pixel_type_list_[i]->GetSingleChunkOrder();
      }
   }


   void AddPixelData(BasePixel* pixel)
   {
      // Add pixels to an array we can access, and we set the pixels Index value
      pixel->pixel_index = pixel_type_counter_;
      pixel_type_list_[pixel_type_counter_] = pixel;
      pixel_type_counter_++;

      pixel_types_[pixel->GetType()] = pixel;
      // Add our colours to our Lookup table
      for (short i = 0; i < pixel->colour_count; i++)
      {
         if (pixel_colour_map_.find(pixel->type_colours[i]) != pixel_colour_map_.end())
         {
            printf("WorldDataHandler already contains a Pixel of type '%s' with Colour Index: '%i'", pixel->Name(), i);
         }
         pixel_colour_map_.insert(std::make_pair(pixel->type_colours[i], pixel));
      }
   }

   WorldDataHandler() = default;;

   WorldDataHandler(WorldDataHandler const&)
   {
   };

   void operator=(WorldDataHandler const&) const
   {
   };
private:
   static WorldDataHandler* instance_;

   short pixel_type_counter_ = 0;
   std::array<BasePixel*, static_cast<short>(E_PixelType::COUNT)> pixel_type_list_;
   std::unordered_map<Uint32, BasePixel*> pixel_colour_map_;
   std::unordered_map<E_PixelType, BasePixel*> pixel_types_;
};
