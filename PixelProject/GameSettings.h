#pragma once
#ifndef _GAME_SETTINGS_
#define _GAME_SETTINGS_
#include "SDL_FontCache/SDL_FontCache.h"
#include "Vec2.h"
#include "PaintManager.h"
#include "DebugStopWatch.h"

#include "ConfigFile.h"

//TODO Should probably wrap this up into an object

struct GameSettings
{
		FC_Font* font;

		PaintManager* paint_manager;
		DebugStopWatch* stop_watch;

		// Temp for Debug
		IVec2 virtual_mouse = IVec2::Zero();

		/* Game Settings that will be global */
		IVec2 screen_size = IVec2(1280, 720);

		float target_frames_per_second = 60;
		float calculated_frame_delay = 1000.0f / 60.0f;

		void LoadSettings(ConfigFile& config) {
				screen_size.x = config.screen_x;
				screen_size.y = config.screen_y;
				target_frames_per_second = config.target_frames_per_second;

				// Apply changes
				calculated_frame_delay = 1000.0f / target_frames_per_second;
		}

		GameSettings() {
				paint_manager = nullptr;
				stop_watch = nullptr;
				font = nullptr;
				screen_size = IVec2::Zero();
		}
};


#endif