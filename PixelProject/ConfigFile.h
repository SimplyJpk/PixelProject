#pragma once
#include "ISerializable.h"

struct ConfigFile final : public ISerializable
{
   short screen_x = 1280;
   short screen_y = 720;

   float target_frames_per_second = 60.0;

   uint8_t target_sand_update_per_seconds = 60;
   uint8_t max_sand_updates_per_frame = 10;

   SaveTypes SaveType() override
   {
      return SaveTypes::Json;
   }

   std::string FilePath() override { return std::string("config"); }

   // Inherited via ISerializable
   virtual void Save(cereal::JSONOutputArchive out_archive) override
   {
      out_archive(CEREAL_NVP(screen_x));
      out_archive(CEREAL_NVP(screen_y));
      out_archive(CEREAL_NVP(target_frames_per_second));
      out_archive(CEREAL_NVP(target_sand_update_per_seconds));
      out_archive(CEREAL_NVP(max_sand_updates_per_frame));
   }

   virtual void Load(cereal::JSONInputArchive in_archive) override
   {
      in_archive(screen_x);
      in_archive(screen_y);
      in_archive(target_frames_per_second);
      in_archive(target_sand_update_per_seconds);
      in_archive(max_sand_updates_per_frame);
   }
};
