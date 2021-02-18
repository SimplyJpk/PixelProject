#include "Game.h"

typedef ShaderManager::ShaderTypes ShaderType;

bool Game::Initialize(SDL_GLContext* gl_context, SDL_Window* gl_window, GameSettings* settings)
{
   game_settings = settings;
   g_context = gl_context;
   g_window = gl_window;

   glViewport(0, 0, settings->screen_size.x, settings->screen_size.y);
   // glMatrixMode(GL_PROJECTION);
   // glLoadIdentity();
   // float aspect = (float)settings->screen_size.x / (float)settings->screen_size.y;
   // glOrtho(-aspect, aspect, -1, 1, -1, 1);

   // glMatrixMode(GL_MODELVIEW);
   // glLoadIdentity();

   if (!ShaderManager::Instance()->CompileShader("default", ShaderType::Vertex, "shaders/default.vert"))
      printf("Failed to generate Vertex Shader");
   if (!ShaderManager::Instance()->CompileShader("default", ShaderType::Fragment, "shaders/default.frag"))
      printf("Failed to generate Frag Shader");

   defaultShader = ShaderManager::Instance()->CreateShaderProgram("default", false);
   game_settings->default_shader = defaultShader;

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

   //TODO Needs to be updated to work with new renderer
   //? game_settings->paint_manager->GeneratePixelTextures(renderer);

   // Initialize ImGUI
   gui_manager = new GuiManager(game_settings, g_window, g_context);
   gui_manager->SetPaintManager(paint_manager);
   //TODO Look into if FC_Font will still work with opengl?
   game_settings->font = nullptr;
   // Input
   input_manager = InputManager::Instance();
   // Camera
   main_cam.SetPerspective(1.0472f, game_settings->aspect_ratio, 0.1f, 100.f);

   // World Sim
   world_sim = new WorldSimulator(game_settings);
   world_sim->paint_manager = paint_manager;

   // Store Camera in World Simulator so we don't have to pass it
   world_sim->cam = &main_cam;
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

   game_settings->stop_watch->AddTimer("SlowestUpdate");
   game_settings->stop_watch->UpdateTime("SlowestUpdate", 0.0f);
   game_settings->stop_watch->AddTimer("FastestUpdate");
   game_settings->stop_watch->UpdateTime("FastestUpdate", 1000.0f);

   auto deltaTime = 0.0f;

   //? Debug Info
   auto frameStart = clock::now();
   auto secondCounter = clock::now();
   int frameCounter = 0;

   while (!input_manager->IsShuttingDown())
   {
      deltaTime = (static_cast<duration>(frameStart - clock::now())).count();
      //? Debug Info
      frameStart = clock::now();

      //? ======
      //! Update
      input_manager->Update();
      if (input_manager->IsShuttingDown()) break;

      main_cam.Update(deltaTime);

      paint_manager->UpdateInput();

      world_sim->UpdateInput();

      world_sim->Update();
      //? main_cam->Update();

      //? Debug Info
      game_settings->stop_watch->UpdateTime("Update_ms", static_cast<duration>(clock::now() - frameStart).count());
      static float average = 0.f;
      auto nanoTime = static_cast<std::chrono::nanoseconds>(clock::now() - frameStart).count() / 1000;
      average += nanoTime;
      static int microUpdate = 0;
      if (nanoTime > game_settings->stop_watch->GetTime("SlowestUpdate"))
         game_settings->stop_watch->UpdateTime("SlowestUpdate", nanoTime);
      if (nanoTime < game_settings->stop_watch->GetTime("FastestUpdate"))
         game_settings->stop_watch->UpdateTime("FastestUpdate", nanoTime);

      if (microUpdate >= 12) {
         game_settings->stop_watch->UpdateTime("Update_micro_avg", average / (microUpdate + 1));
         average = 0.f;
         microUpdate = 0;
      }
      else { microUpdate++; }

      auto drawStart = clock::now();

      //? ======
      //! Render
      // Clear Screen
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gui_manager->NewGuiFrame();

      world_sim->Draw(&main_cam);
      // Draw some stock GUI with ImGUI
      gui_manager->DrawGui();

      gui_manager->FinishGuiFrame();

      game_settings->stop_watch->UpdateTime("Draw", static_cast<duration>(clock::now() - drawStart).count());

      SDL_GL_SwapWindow(g_window);

      //? Debug Info
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
