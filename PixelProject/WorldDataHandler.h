#pragma once
#include "PixelTypeIncludes.h"

#include <unordered_map>
#include <array>
#include <gl/GLEW.h>


#include "ColourUtility.h"
#include "PixelDataMasks.h"
#include "Shader.h"
#include "StringUtility.h"

//TODO Should this be a singleton? or a static class? The contained information shouldn't change.
class WorldDataHandler
{
public:
   static WorldDataHandler& Instance();

   // Information from Pixel Colours
   // Returns the name of a PixelType from a Pixel Colour
   char* GetPixelName(const Uint32 pixel) { return pixel_colour_map_[pixel]->pixel_name; }
   // Returns the BasePixel based on the pixel colour passed in
   inline BasePixel* GetPixelFromPixelColour(const Uint32 pixel) { return pixel_colour_map_[pixel]; }

   // Returns base pixel from the Index provided, only use when calling using Pixel->PixelIndex
   BasePixel* GetPixelFromIndex(const short index) { return pixel_type_list_[index]; }

   // Returns random colour based on the type passed in.
   Uint32 GetColorFromType(const E_PixelType index) { return pixel_types_[index]->GetRandomColour(); }

   // Information from PixelType (Enum)
   BasePixel* GetPixelFromType(const E_PixelType type) { return pixel_types_[type]; }
   // Returns the name of a PixelType from the PixelType passed in
   char* GetPixelName(const E_PixelType type) { return pixel_types_[type]->pixel_name; }

   // Returns the number of main pixel types that exist in the game
   int PixelTypeCount() const { return pixel_type_counter_; }

   void FillWithPixelUpdateOrders(short* output[])
   {
      for (int i = 0; i < pixel_type_counter_; i++)
      {
         output[i] = pixel_type_list_[i]->GetSingleChunkOrder();
      }
   }

   Uint32 GetNewPixelOfType(const E_PixelType type)
   {
      return pixel_types_[type]->GetNewPixel();
   }


   void AddPixelData(BasePixel* pixel)
   {
      // Add pixels to an array we can access, and we set the pixels Index value
      pixel->pixel_index = pixel_type_counter_;
      pixel_type_list_[pixel_type_counter_] = pixel;
      pixel_type_counter_++;

      pixel_types_[pixel->pixel_type] = pixel;
      // Add our colours to our Lookup table
      for (short i = 0; i < pixel->colour_count; i++)
      {
         if (pixel_colour_map_.find(pixel->type_colours[i]) != pixel_colour_map_.end())
         {
            printf("WorldDataHandler already contains a Pixel of type '%s' with Colour Index: '%i'", pixel->pixel_name, i);
         }
         pixel_colour_map_.insert(std::make_pair(pixel->type_colours[i], pixel));
      }
   }

   void SetUniformData(Shader* program)
   {
      program->UseProgram();
      GLint myLoc;
      int programID = program->GetProgramID();
      // Set PixelData
      for (int i = 0; i < pixel_type_counter_; i++)
      {
         auto pixel = pixel_type_list_[i];
         std::string locationString = Utility::string_format("u_Pixels[%i].colour_count", pixel->pixel_index);
         myLoc = program->GetUniformLocation(locationString);
         glProgramUniform1i(programID, myLoc, pixel->colour_count);
         for (int j = 0; j < pixel->colour_count; j++)
         {
            glProgramUniform4fv(programID, myLoc+(j+1), 4, pixel->render_colours[j]);
         }
      }

      //TODO Automate? Make this better?
      // Set MaskData
      myLoc = program->GetUniformLocation("u_PixelMask.index");
      glProgramUniform1ui(programID, myLoc, PBit::IndexBits());
      myLoc = program->GetUniformLocation("u_PixelMask.lifetime");
      glProgramUniform1ui(programID, myLoc, PBit::LifetimeBits());
      myLoc = program->GetUniformLocation("u_PixelMask.behaviour");
      glProgramUniform1ui(programID, myLoc, PBit::BehaviourBits());
      myLoc = program->GetUniformLocation("u_PixelMask.light");
      glProgramUniform1ui(programID, myLoc, PBit::LightBits());

      // Set MaskData
      myLoc = program->GetUniformLocation("u_PixelBitOffset.index");
      glProgramUniform1ui(programID, myLoc, PBit::IndexDepth());
      myLoc = program->GetUniformLocation("u_PixelBitOffset.lifetime");
      glProgramUniform1ui(programID, myLoc, PBit::LifetimeDepth());
      myLoc = program->GetUniformLocation("u_PixelBitOffset.behaviour");
      glProgramUniform1ui(programID, myLoc, PBit::BehaviourDepth());
      myLoc = program->GetUniformLocation("u_PixelBitOffset.light");
      glProgramUniform1ui(programID, myLoc, PBit::LightDepth());
   }

   WorldDataHandler(const WorldDataHandler&) = delete;
   WorldDataHandler(WorldDataHandler&&) = delete;
   void operator=(const WorldDataHandler&) = delete;
   void operator=(WorldDataHandler&&) = delete;

private:
   WorldDataHandler()
   {
      AddPixelData(new SpacePixel());
      AddPixelData(new GroundPixel());
      AddPixelData(new SandPixel());
      AddPixelData(new WaterPixel());
      AddPixelData(new WoodPixel());
      AddPixelData(new OilPixel());
      AddPixelData(new FirePixel());
      AddPixelData(new AcidPixel());
      AddPixelData(new GoldPixel());
      AddPixelData(new SteamPixel());
      AddPixelData(new SourceBlock(GetPixelFromType(E_PixelType::Sand)));
      AddPixelData(new SourceBlock(GetPixelFromType(E_PixelType::Water)));
      AddPixelData(new SourceBlock(GetPixelFromType(E_PixelType::Oil)));
      AddPixelData(new SourceBlock(GetPixelFromType(E_PixelType::Fire)));
      AddPixelData(new SourceBlock(GetPixelFromType(E_PixelType::Acid)));
      AddPixelData(new SourceBlock(GetPixelFromType(E_PixelType::Gold)));
      AddPixelData(new SourceBlock(GetPixelFromType(E_PixelType::Steam)));
   }
   ~WorldDataHandler() = default;

   short pixel_type_counter_ = 0;
   std::array<BasePixel*, static_cast<short>(E_PixelType::COUNT)> pixel_type_list_{ nullptr };
   std::unordered_map<Uint32, BasePixel*> pixel_colour_map_;
   std::unordered_map<E_PixelType, BasePixel*> pixel_types_;
};
