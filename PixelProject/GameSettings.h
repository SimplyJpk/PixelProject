#pragma once
#include "SDL_FontCache/SDL_FontCache.h"
#include "Vec2.h"
#include "PaintManager.h"
#include "DebugStopWatch.h"

#include "ConfigFile.h"

struct GameSettings
{
   FC_Font* font = nullptr;

   PaintManager* paint_manager = nullptr;
   DebugStopWatch* stop_watch = nullptr;

   // Temp for Debug
   IVec2 virtual_mouse = IVec2::Zero();

   /* Game Settings that will be global */
   IVec2 screen_size = IVec2(1280, 720);

   float target_frames_per_second = 60.0f;
   float calculated_frame_delay = 1000.0f / target_frames_per_second;

   void LoadSettings(ConfigFile& config)
   {
      screen_size.x = config.screen_x;
      screen_size.y = config.screen_y;
      target_frames_per_second = config.target_frames_per_second;

      // Apply changes
      calculated_frame_delay = 1000.0f / target_frames_per_second;
   };
};
