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



   ////? typedef std::chrono::steady_clock clock;
   ////? typedef std::chrono::duration<float, std::milli> duration;
   ////? 
   ////? auto frameStart = clock::now();
   ////? auto frameEnd = clock::now();
   ////? auto secondCounter = clock::now();
   ////? int frameCounter = 0;
   ////? int total_frame_count = 0;
   ////? float total_update_cost = 0.0f;

   ////? DebugStopWatch stopWatch;
   ////? stopWatch.SetTimerState("Update", true);
   ////? //! General Initialization
   ////? stopWatch.AddTimer("Init", false);
   ////? stopWatch.AddTimer("Start", false);
   ////? stopWatch.AddTimer("Update", true);
   ////? stopWatch.AddTimer("Draw", true);
   ////? stopWatch.AddTimer("FrameTime", true);
   ////? stopWatch.AddTimer("Input", false);
   ////? stopWatch.AddTimer("CurrentFPS", true);
   ////? stopWatch.AddTimer("PotentialFPS", true);
   ////? stopWatch.AddTimer("FullFrameTime", true);
   ////? 
   ////? stopWatch.AddTimer("AVG_UpdateTime", true);

   ////? // Initalize our settings
   ////
   ////? settings->stop_watch = &stopWatch;
   ////? 
   ////? settings->paint_manager->GeneratePixelTextures(renderer);

   ////? 
   ////? FC_Font* font = FC_CreateFont();
   ////? FC_LoadFont(font, renderer, "fonts/FreeSans.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
   ////? settings->font = font;
   ////? 
   ////? auto clock_second_start = clock::now();
   ////? 

   ////? stopWatch.StoreTime("Init");
   ////? //! General Initialization of Engine Complete
   ////? 
   ////? //! Start
   ////? stopWatch.UpdateTime("Start");
   ////? 
   ////? 
   ////? //TODO REMOVE THIS
   ////? bool debugFrameByFrameUpdate = false;

   ////? stopWatch.StoreTime("Start");
   ////! Start Finished
   //// Update
   //while (!input_manager->IsShuttingDown())
   //{
   //   //? frameStart = std::chrono::steady_clock::now();
   //   //? // Check Inputs
   //   input_manager->Update();
   //   if (input_manager->IsShuttingDown()) break;
   //   //? stopWatch.StoreTime("Input", static_cast<duration>(clock::now() - frameStart).count());
   //   //? 
   //   //? // Debug
   //   //? if (input_manager->GetKeyDown(KeyCode::T))
   //   //? {
   //   //?    total_update_cost = 0;
   //   //?    total_frame_count = 0;
   //   //? }
   //   //? 
   //   //? total_frame_count++;
   //   //? 
   //   //? //TODO Move this somewhere better?
   //   settings->paint_manager->UpdateInput();
   //   world_sim->UpdateInput();
   //   //? 
   //   //? if (input_manager->GetKeyButton(KeyCode::_0))
   //   //?    debugFrameByFrameUpdate = !debugFrameByFrameUpdate;
   //   //? // Updates
   //   //? if (!debugFrameByFrameUpdate || input_manager->GetKeyDown(KeyCode::Down))
   //   //? {
   //   world_sim->Update();
   //   //? }
   //   mainCam->Update();
   //   //? 
   //   //? stopWatch.StoreTime("Update", static_cast<duration>(clock::now() - frameStart).count());
   //   //? total_update_cost += static_cast<std::chrono::nanoseconds>(clock::now() - frameStart).count() / 1000;
   //   //? 
   //   //? stopWatch.StoreTime("AVG_UpdateTime", total_update_cost / total_frame_count);
   //   //? 
   //   //? auto drawStart = clock::now();
   //   //? //x stopWatch.UpdateTime("Draw");
   //   //? //
   //   // Clear Screen
   //   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //   if (!gui_manager->NewGuiFrame())
   //   {
   //      printf("ImGUI Failed to create new frame");
   //   }
   //   //? SDL_RenderClear(renderer);
   //   //? 
   //   world_sim->Draw(mainCam);
   //   //? 
   //   //? //? Delete this? DEBUG
   //   //? SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
   //   //? SDL_RenderDrawLine(renderer, settings->virtual_mouse.x, settings->virtual_mouse.y, settings->virtual_mouse.x,
   //   //?    settings->virtual_mouse.y);
   //   //? 
   //   //? 
   //   //? // SDL_RenderCopy()
   //   //? 
   //   //? // Draw GUI
   //   gui_manager->DrawGui();
   //   //? 
   //   //? settings->paint_manager->DrawTexture(renderer, IVec2(200,200), E_PixelType::COUNT);
   //   //? 
   //   //? // Render
   //   //? SDL_RenderPresent(renderer);
   //   //? 
   //   //? stopWatch.StoreTime("Draw", static_cast<duration>(clock::now() - drawStart).count());
   //   //? 
   //   //? // Wait a bit
   //   //? 
   //   //? frameEnd = clock::now();
   //   //? duration time = frameEnd - frameStart;
   //   //? const float currentFrameDuration = time.count();
   //   //? 
   //   //? stopWatch.StoreTime("FrameTime", currentFrameDuration);
   //   //? stopWatch.StoreTime("PotentialFPS", 1000 / static_cast<duration>(clock::now() - frameStart).count());
   //   //? 
   //   //? frameCounter++;
   //   //? if (static_cast<duration>(clock::now() - secondCounter).count() > 1000)
   //   //? {
   //   //?    stopWatch.StoreTime("CurrentFPS", frameCounter);
   //   //?    secondCounter = clock::now();
   //   //?    frameCounter = 0;
   //   //? }
   //   //? 
   //   //? if (currentFrameDuration < settings->calculated_frame_delay)
   //   //? {
   //   //?    SDL_Delay(static_cast<int>(settings->calculated_frame_delay - currentFrameDuration));
   //   //?    while (static_cast<duration>(clock::now() - frameStart).count() < settings->calculated_frame_delay) {}
   //   //? }
   //   //? 
   //   //? 
   //   //? stopWatch.StoreTime("FullFrameTime", static_cast<duration>(clock::now() - frameStart).count());

   //   gui_manager->FinishGuiFrame();

   //   SDL_GL_SwapWindow(window);
   //}

   //// Free all our resources
   ////? FC_FreeFont(font);
   ////? ImGuiSDL::Deinitialize();
   ////? SDL_DestroyRenderer(renderer);

   SDL_DestroyWindow(g_window);
   SDL_Quit();
   return 0;
}

void PrintInGameCommands()
{
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

   // Create a new Window to use
   SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
   g_window = SDL_CreateWindow("Pixel Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, settings->screen_size.x, settings->screen_size.y, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

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
      //Use Vsync
      if (SDL_GL_SetSwapInterval(1) < 0)
      {
         printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
      }

      //Initialize Projection Matrix
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      //Initialize Modelview Matrix
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      //Initialize clear color
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

   }

   // Can only init Glew after openGL context has been created.
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