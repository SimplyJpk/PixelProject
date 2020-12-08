#ifndef _GAME_SETTINGS_
#define _GAME_SETTINGS_

#include "SDL_FontCache/SDL_FontCache.h"
#include "Vec2.h"

//TODO Should probably wrap this up into an object

struct GameSettings
{
		IVec2 _CONFIG_SCREEN_SIZE;
		FC_Font* _font;

		GameSettings() {
				_font = nullptr;
				_CONFIG_SCREEN_SIZE = IVec2::Zero();
		}
};


#endif