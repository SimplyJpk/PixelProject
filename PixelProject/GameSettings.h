#pragma once
#include <gl/GLEW.h>

#include "SDL_FontCache/SDL_FontCache.h"
#include "Vec2.h"
#include "DebugStopWatch.h"

#include "ConfigFile.h"

struct GameSettings
{
   GLint default_shader;
   FC_Font* font = nullptr;

   DebugStopWatch* stop_watch = nullptr;

   // Temp for Debug
   IVec2 virtual_mouse = IVec2::Zero();

   /* Game Settings that will be global */
   IVec2 screen_size = IVec2(1280, 720);
   float aspect_ratio = screen_size.x / screen_size.y;

   float target_frames_per_second = 60.0f;
   float calculated_frame_delay = 1000.0f / target_frames_per_second;

   void LoadSettings(ConfigFile& config)
   {
      screen_size.x = config.screen_x;
      screen_size.y = config.screen_y;
      // Set aspect ratio for future use
      aspect_ratio = screen_size.x / static_cast<float>(screen_size.y);
      // Target updates from config (FPS)
      target_frames_per_second = config.target_frames_per_second;

      calculated_frame_delay = 1000.0f / target_frames_per_second;
   };
};
