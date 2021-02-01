#include "Game.h"

typedef ShaderManager::ShaderTypes ShaderType;

bool Game::Initialize(SDL_GLContext* gl_context, SDL_Window* gl_window, GameSettings* settings)
{
   game_settings = settings;
   g_context = gl_context;
   g_window = gl_window;
   
   ShaderManager::Instance()->CompileShader("default", ShaderType::Vertex, "shaders/default.vert");
   ShaderManager::Instance()->CompileShader("default", ShaderType::Fragment, "shaders/default.frag");
   defaultShader = ShaderManager::Instance()->CreateShaderProgram("default", false);

   //TODO Need to find a better place for this. This needs to be done before Paint Manager in instanced.
   WorldDataHandler::Instance()->AddPixelData(new SpacePixel());
   WorldDataHandler::Instance()->AddPixelData(new GroundPixel());
   WorldDataHandler::Instance()->AddPixelData(new SandPixel());
   WorldDataHandler::Instance()->AddPixelData(new WaterPixel());
   WorldDataHandler::Instance()->AddPixelData(new WoodPixel());
   WorldDataHandler::Instance()->AddPixelData(new OilPixel());
   WorldDataHandler::Instance()->AddPixelData(new FirePixel());
   WorldDataHandler::Instance()->AddPixelData(new AcidPixel());

   paint_manager = new PaintManager();
   game_settings->screen_size = IVec2(game_settings->screen_size.x, game_settings->screen_size.y);

   //TODO Needs to be updated to work with new renderer
   //? game_settings->paint_manager->GeneratePixelTextures(renderer);

   // Initialize ImGUI
   gui_manager = new GuiManager(game_settings, g_window, g_context);
   gui_manager->SetPaintManager(paint_manager);
   //TODO Look into if FC_Font will still work with opengl?
   game_settings->font = nullptr;
   // Input
   input_manager = InputManager::Instance();
   // World Sim
   world_sim = new WorldSimulator(game_settings);
   world_sim->paint_manager = paint_manager;

   //TODO Fix Camera
   main_cam = new Camera(IVec2(0, 0), game_settings);
   main_cam->Start();

   // Store Camera in World Simulator so we don't have to pass it
   world_sim->cam = main_cam;
   world_sim->Start();

   return true;
}

void Game::Run()
{
   //? Debug Info?
   typedef std::chrono::steady_clock clock;
   typedef std::chrono::duration<float, std::milli> duration;

   //? Debug Info
   game_settings->stop_watch->AddTimer("Update_ms");
   game_settings->stop_watch->AddTimer("Update_micro_avg");
   game_settings->stop_watch->AddTimer("Draw");
   game_settings->stop_watch->AddTimer("FrameTime");
   game_settings->stop_watch->AddTimer("CurrentFPS");

   //? Debug Info
   auto frameStart = clock::now();
   auto secondCounter = clock::now();
   int frameCounter = 0;

   while (!input_manager->IsShuttingDown())
   {
      //? Debug Info
      frameStart = clock::now();

      //! Update
      input_manager->Update();
      if (input_manager->IsShuttingDown()) break;

      paint_manager->UpdateInput();

      world_sim->UpdateInput();

      world_sim->Update();
      main_cam->Update();

      //? Debug Info
      game_settings->stop_watch->UpdateTime("Update_ms", static_cast<duration>(clock::now() - frameStart).count());
      static float average = 0.f;
      average += static_cast<std::chrono::nanoseconds>(clock::now() - frameStart).count() / 1000;
      static int microUpdate = 0;
      if (microUpdate >= 12) {
         game_settings->stop_watch->UpdateTime("Update_micro_avg", average / (microUpdate + 1));
         average = 0.f;
         microUpdate = 0;
      }
      else { microUpdate++; }
      auto drawStart = clock::now();

      //! Render
      // Clear Screen
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gui_manager->NewGuiFrame();

      world_sim->Draw(main_cam);
      // Draw some stock GUI with ImGUI
      gui_manager->DrawGui();


      gui_manager->FinishGuiFrame();

      game_settings->stop_watch->UpdateTime("Draw", static_cast<duration>(clock::now() - drawStart).count());

      SDL_GL_SwapWindow(g_window);

      game_settings->stop_watch->UpdateTime("FrameTime", static_cast<duration>(clock::now() - frameStart).count());

      frameCounter++;
      if (static_cast<duration>(clock::now() - secondCounter).count() > 1000)
      {
         game_settings->stop_watch->UpdateTime("CurrentFPS", frameCounter);
         secondCounter = clock::now();
         frameCounter = 0;
      }

   }

   //TODO Should do some cleanup for destruction?
}
