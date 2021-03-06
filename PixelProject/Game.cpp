#include "Game.h"

typedef ShaderManager::ShaderTypes ShaderType;

bool Game::Initialize(SDL_GLContext* gl_context, SDL_Window* gl_window, GameSettings* settings)
{
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   game_settings = settings;
   g_context = gl_context;
   g_window = gl_window;

   glViewport(0, 0, settings->screen_size.x, settings->screen_size.y);

   if (!ShaderManager::Instance()->CompileShader("orthoWorld", ShaderType::Vertex, "shaders/orthoWorld.vert"))
      printf("Failed to generate Vertex Shader");
   if (!ShaderManager::Instance()->CompileShader("orthoWorld", ShaderType::Fragment, "shaders/orthoWorld.frag"))
      printf("Failed to generate Frag Shader");

   defaultShader = ShaderManager::Instance()->CreateShaderProgram("orthoWorld", false);
   game_settings->default_shader = defaultShader;

   paint_manager = new PaintManager();

   // Initialize ImGUI
   gui_manager = new GuiManager(game_settings, g_window, g_context);
   gui_manager->SetPaintManager(paint_manager);


   //TODO Look into if FC_Font will still work with opengl?
   game_settings->font = nullptr;
   // Input
   input_manager = InputManager::Instance();
   // Camera
   main_cam.SetOrtho(0, game_settings->screen_size.x, game_settings->screen_size.y, 0);
   //x main_cam.SetPerspective(1.0472f, game_settings->aspect_ratio, 0.1f, 100.f);

   //TODO V This should probably be static, it doesn't really need to know what is going on in the world, only where it is.
   world_generator_ = new WorldGenerator();
   // World Sim
   world_sim = new WorldSimulator(game_settings, world_generator_);
   world_sim->paint_manager = paint_manager;
   // Call this after the World_Sim so we know everything is ready.
   paint_manager->GeneratePixelTextures(game_settings);

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
   game_settings->stop_watch->AddTimer("Fixed_ms");

   game_settings->stop_watch->AddTimer("SlowestUpdate");
   game_settings->stop_watch->UpdateTime("SlowestUpdate", 0.0f);
   game_settings->stop_watch->AddTimer("FastestUpdate");
   game_settings->stop_watch->UpdateTime("FastestUpdate", 1000.0f);

   auto deltaClock = clock::now();
   double deltaTime = 0.0;
   double fixedRemainingTime = 0.0;

   //? Debug Info
   auto frameStart = clock::now();
   auto secondCounter = clock::now();
   int frameCounter = 0;

   while (!input_manager->IsShuttingDown())
   {
      deltaTime = static_cast<duration>(clock::now() - deltaClock).count();
      deltaClock = clock::now();
      frameStart = clock::now();

      fixedRemainingTime+= deltaTime;

      uint8_t frameFixedStepCounter = 0;
      while (fixedRemainingTime > game_settings->target_sand_update_time) {
         world_sim->FixedUpdate();
         fixedRemainingTime -= game_settings->target_sand_update_time;
         // Prevents lockup of game during intensive updates, but should hopefully allow recovery.
         frameFixedStepCounter++;
         game_settings->stop_watch->UpdateTime("Fixed_ms", static_cast<duration>(clock::now() - frameStart).count());
         if (frameFixedStepCounter >= game_settings->max_sand_updates_per_frame)
            break;
      }

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

      paint_manager->DrawPaintGUI(&main_cam);
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
