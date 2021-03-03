//TODO Clean this up a bit
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "GameSettings.h"
#include "ConfigFile.h"

#include "IO.h"

#include "Game.h"

#include <iostream>

void PrintInGameCommands();
bool CreateWindowAndContext(GameSettings* settings);

SDL_GLContext g_context;
SDL_Window* g_window;
Game* game;

int main(int argc, char** argv)
{
   bool success = true;

   //TODO Implement a check for all required directories?
   IO::create_file_if_not_exist("/data/");

   // Load our Config class which we can then use to construct out GameSettings
   printf("Loading config file..\n");
   ConfigFile config;
   config.StartLoad(config.FilePath().c_str(), true);

   PrintInGameCommands();

   // Generate our game settings with the Serialized config file
   GameSettings* settings = new GameSettings();
   settings->LoadSettings(config);
   settings->stop_watch = new DebugStopWatch();

   if (!CreateWindowAndContext(settings))
      success = false;

   game = new Game();
   if (!success || !game->Initialize(&g_context, g_window, settings))
   {
      printf("PixelProject failed to Initialize");
      success = false;
   }
   else
   {
      game->Run();
   }

   SDL_DestroyWindow(g_window);
   SDL_Quit();
   return 0;
}

void PrintInGameCommands()
{
   printf("/--\t\tSome of these are likely wrong..\n");
   printf("/--\t\tCommands\n");
   printf("|- TAB\t\t- Change Pixel type\n");
   printf("|- Mouse1\t- Draw Selected Pixel\n");
   printf("|- Mouse2\t- Clear PenSize of Pixels\n");
   printf("|- J\t\t- +1 PenSize\n");
   printf("|- K\t\t- -1 PenSize\n");
   printf("|- N\t\t- Increase Sand Spawn\n");
   printf("|- M\t\t- Slow Sand Spawn\n");
   printf("|- D\t\t- Hide Chunk Borders\n");
   printf("|- S\t\t- Save World\n");
   printf("|- L\t\t- Load World\n");
   printf("|- E\t\t- Zoom Out\n");
   printf("|- R\t\t- Zoom In\n");
   printf("|- SPACE\t- Toggle Sand Spawn\n");
   printf("|- ENTER\t- Restart World (Clears Everything)\n\n");
   printf("|- 0\t\t- Toggle FrameByFrame Update\n");
   printf("|--> Down Arrow\t- Progress by 1 Frame\n");
   printf("|--> Up Arrow\t- Toggle Pixel Data Print\n");
   printf("|- Esc\t\t- Close Game\n\n");
   printf("|- TFGH\t\t- Virtual Mouse Directions\n");
   printf("|- Comma\t- Draw at Virtual Mouse\n");
   printf("|- Period\t- Draw at Mouse\n");
}

bool CreateWindowAndContext(GameSettings* settings)
{
   bool success = true;

   SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
   g_window = SDL_CreateWindow(
      "Pixel Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      settings->screen_size.x, settings->screen_size.y,
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
   );

   if (g_window == nullptr) {
      printf("Window could not be created!\nSDL Err: %s\n", SDL_GetError());
      success = false;
   }
   g_context = SDL_GL_CreateContext(g_window);
   if (g_context == nullptr)
   {
      printf("OpenGL context could not be created! SDL Err: %s\n", SDL_GetError());
      success = false;
   }
   else
   {
      // Vsync cause we're bad
      if (SDL_GL_SetSwapInterval(1) < 0)
      {
         printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
      }
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
   }
   // Init glew, has to be done after context
   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      /* Problem: glewInit failed, something is seriously wrong. */
      std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
      success = false;
   }
   std::cerr << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
   return success;
}