#pragma once
#include <gl/GLEW.h>

#include "SDL_FontCache/SDL_FontCache.h"
#include "Math.h"
#include "DebugStopWatch.h"

#include "ConfigFile.h"

#include "Constants.h"
#include "Shader.h"

struct GameSettings
{
   Shader* default_shader;
   FC_Font* font = nullptr;

   //TODO Move or remove this later
   DebugStopWatch stop_watch;

   // Temp for Debug
   IVec2 virtual_mouse = IVec2(PixelProject::Constant::chunk_size_x / 2, PixelProject::Constant::chunk_size_y / 2);

   /* Game Settings that will be global */
   IVec2 screen_size = IVec2(1280, 720);
   float aspect_ratio = static_cast<float>(screen_size.x) / static_cast<float>(screen_size.y);

   // Game Fixed TimeStep
   uint8_t target_sand_updates_per_seconds = 60;
   double target_sand_update_time = 1000.0 / target_sand_updates_per_seconds;

   float target_frames_per_second = 60.0f;
   float calculated_frame_delay = 1000.0f / target_frames_per_second;
   // Max number of updates we want the fixed update to run, this prevents complete lockup.
   uint8_t max_sand_updates_per_frame = 10;

   void LoadSettings(ConfigFile& config)
   {
      screen_size.x = config.screen_x;
      screen_size.y = config.screen_y;
      // Set aspect ratio for future use
      aspect_ratio = screen_size.x / static_cast<float>(screen_size.y);
      // Target updates from config (FPS)
      target_frames_per_second = config.target_frames_per_second;

      calculated_frame_delay = 1000.0f / target_frames_per_second;

      target_sand_updates_per_seconds = config.target_sand_update_per_seconds;
      target_sand_update_time = 1000.0 / target_sand_updates_per_seconds;

      max_sand_updates_per_frame = config.max_sand_updates_per_frame;
   };
};
