#ifndef _GAME_SETTINGS_
#define _GAME_SETTINGS_

#include "SDL_FontCache/SDL_FontCache.h"
#include "Vec2.h"
#include "PaintManager.h"
#include "DebugStopWatch.h"

//TODO Should probably wrap this up into an object

struct GameSettings
{
		IVec2 Screen_Size;
		FC_Font* _font;

		PaintManager* _paintManager;
		DebugStopWatch* _stopWatch;

		// Temp for Debug
		IVec2 VirtualMouse = IVec2::Zero();

		GameSettings() {
				_font = nullptr;
				Screen_Size = IVec2::Zero();

				_paintManager = new PaintManager();
		}
};


#endif