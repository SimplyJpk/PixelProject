#ifndef _GAME_SETTINGS_
#define _GAME_SETTINGS_

#include "Vec2.h"

//TODO Should probably wrap this up into an object

struct GameSettings
{
		IVec2 _CONFIG_SCREEN_SIZE;

		GameSettings() {
				_CONFIG_SCREEN_SIZE = IVec2::Zero();
		}
};


#endif