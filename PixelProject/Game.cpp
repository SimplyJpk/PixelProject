#include "Game.h"

#include "RenderTarget.h"

bool Game::Initialize(SDL_GLContext* gl_context, SDL_Window* gl_window, GameSettings* settings)
{
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   game_settings = settings;
   g_context = gl_context;
   g_window = gl_window;

   glViewport(0, 0, settings->screen_size.x, settings->screen_size.y);

   // Render Target / Post Processing
   if (!ShaderManager::Instance().ShaderFromFiles(MVertex | MFragment, "renderTarget", "shaders/rendertarget/default"))
      printf("Failed to generate RenderTarget Shader\n");
   ShaderManager::Instance().CreateShaderProgram("renderTarget", false);
   // World Shaders
   if (!ShaderManager::Instance().ShaderFromFiles(MVertex | MFragment, "orthoWorld", "shaders/orthoWorld"))
      printf("Failed to generate orthoWorld Shader\n");
   defaultShader = ShaderManager::Instance().CreateShaderProgram("orthoWorld", false);

   game_settings->default_shader = defaultShader;

   paint_manager = new PaintManager();

   // Generate Uniform Data for rendering, this has to be done after paint_manager currently due to ordering.
   WorldDataHandler::Instance().SetUniformData(defaultShader);
   
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
   auto& stop_watch = game_settings->stop_watch;
   stop_watch.AddToGroup("Frame Data", "Update_ms");
   stop_watch.AddToGroup("Frame Data", "Update_micro_avg");
   stop_watch.AddToGroup("Frame Data", "Draw");
   stop_watch.AddToGroup("Frame Data", "FrameTime");
   stop_watch.AddToGroup("Frame Data", "CurrentFPS");
   stop_watch.AddToGroup("Frame Data", "Fixed_ms");
   stop_watch.AddToGroup("Frame Data", "SlowestUpdate", -1.0f);
   stop_watch.AddToGroup("Frame Data", "FastestUpdate", 1000.0f);

   auto deltaClock = clock::now();
   double deltaTime = 0.0;
   double fixedRemainingTime = 0.0;

   //? Debug Info
   auto frameStart = clock::now();
   auto secondCounter = clock::now();
   int frameCounter = 0;

   RenderTarget renderTarget(game_settings->screen_size.x, game_settings->screen_size.y);
   renderTarget.SetShader(&ShaderManager::Instance().GetShader("renderTarget"));

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
         stop_watch.UpdateTime("Fixed_ms", static_cast<duration>(clock::now() - frameStart).count());
         if (frameFixedStepCounter >= game_settings->max_sand_updates_per_frame)
            break;
      }

      //? ======
      //! Update
      input_manager->Update();
      if (input_manager->IsShuttingDown()) break;

      //TODO maybe don't leave this here
      if (input_manager->GetKeyDown(KeyCode::U))
         renderTarget.kernal_state++;
      else if (input_manager->GetKeyDown(KeyCode::I))
         renderTarget.kernal_state--;

      main_cam.Update(deltaTime);

      paint_manager->UpdateInput();

      world_sim->UpdateInput();

      world_sim->Update();
      //? main_cam->Update();

      //? Debug Info
      stop_watch.UpdateTime("Update_ms", static_cast<duration>(clock::now() - frameStart).count());
      static float average = 0.f;
      auto nanoTime = static_cast<std::chrono::nanoseconds>(clock::now() - frameStart).count() / 1000;
      average += nanoTime;
      static int microUpdate = 0;
      if (nanoTime > stop_watch.GetTime("SlowestUpdate"))
         stop_watch.UpdateTime("SlowestUpdate", nanoTime);
      if (nanoTime < stop_watch.GetTime("FastestUpdate"))
         stop_watch.UpdateTime("FastestUpdate", nanoTime);

      if (microUpdate >= 12) {
         stop_watch.UpdateTime("Update_micro_avg", average / (microUpdate + 1));
         average = 0.f;
         microUpdate = 0;
      }
      else { microUpdate++; }

      stop_watch.UpdateTime("NanoUpdate", nanoTime);

      auto drawStart = clock::now();

      //? ======
      //! Render
      // Clear Screen
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gui_manager->NewGuiFrame();

      renderTarget.BindRenderTarget();

      world_sim->Draw(&main_cam);

      // Draw our World before we draw GUI over it
      renderTarget.DrawTargetQuad(0, 0);

      // Draw some stock GUI with ImGUI
      gui_manager->DrawGui();

      paint_manager->DrawPaintGUI(&main_cam);
      gui_manager->FinishGuiFrame();

      stop_watch.UpdateTime("Draw", static_cast<duration>(clock::now() - drawStart).count());


      SDL_GL_SwapWindow(g_window);

      //? Debug Info
      stop_watch.UpdateTime("FrameTime", static_cast<duration>(clock::now() - frameStart).count());

      frameCounter++;
      if (static_cast<duration>(clock::now() - secondCounter).count() > 1000)
      {
         stop_watch.UpdateTime("CurrentFPS", frameCounter);
         secondCounter = clock::now();
         frameCounter = 0;
      }

   }

   //TODO Should do some cleanup for destruction?
}
