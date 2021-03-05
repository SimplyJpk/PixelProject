#pragma once
#include <SDL.h>

#include "GameSettings.h"

#include "GUIManager.h"
#include "InputManager.h"
#include "WorldSimulator.h"
#include "WorldGenerator.h"

#include "PixelTypeIncludes.h"
#include "FlyCamera.h"
#include "ShaderManager.h"

#include "Vec2.h"

class Game
{
   SDL_GLContext* g_context;
   SDL_Window* g_window;

   InputManager* input_manager;
   GuiManager* gui_manager;

   GameSettings* game_settings;

   PaintManager* paint_manager;

   WorldSimulator* world_sim;
   WorldGenerator* world_generator_;
   FlyCamera main_cam;

   GLint defaultShader;

public:
   bool Initialize(SDL_GLContext* gl_context, SDL_Window* gl_window, GameSettings* settings);
   void Run();
};
