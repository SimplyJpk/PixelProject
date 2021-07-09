#pragma once
// ImGUI
#include <gl/GLEW.h>
#include "imgui.h"
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>
#include <SDL.h>

#include "GameSettings.h"
#include "MemoryUsage.h"
#include "Constants.h"
#include "PaintManager.h"

class GuiManager
{
public:
   MemoryUsage mem_usage;
   //Temp Colours
   ImVec4 red = ImVec4(1, 0, 0, 1);
   ImVec4 green = ImVec4(0, 1, 0, 1);

   SDL_Window* g_window;

   void SetPaintManager(PaintManager* painter) {paint_manager_ = painter; }

   static constexpr short plot_data_size = 400;

   float plotMemoryData[plot_data_size]{ 0 };
   float plotCPUData[plot_data_size]{0};

   GuiManager(GameSettings* settings, SDL_Window* window, SDL_GLContext* context)
   {
      settings_ = settings;

      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO(); (void)io;

      ImGui::StyleColorsDark();

      ImGui_ImplSDL2_InitForOpenGL(window, context);
      ImGui_ImplOpenGL3_Init();

      ImGui::SetWindowSize("Debug Window", ImVec2(240, 240));
      ImGui::SetWindowPos("Debug Window", ImVec2(settings_->screen_size.x - 245, 15));

      ImGui::SetWindowSize("Frame Data", ImVec2(240, 240));
      ImGui::SetWindowPos("Frame Data", ImVec2(settings_->screen_size.x - 245, 275));
#if DEBUG_GAME
      ImGui::SetWindowSize("WorldSimulator DEBUG", ImVec2(240, 240));
      ImGui::SetWindowPos("WorldSimulator DEBUG", ImVec2(settings_->screen_size.x - 545, 275));
#endif
      g_window = window;

      for(int i = 0; i < plot_data_size; i++)
      {
         plotMemoryData[i] = static_cast<float>(mem_usage.ReturnMemoryUsed() / 1000.0f);
      }
   }

   void NewGuiFrame() const
   {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame(g_window);
      ImGui::NewFrame();
   }

   // Finializes rendering so it can be drawn.
   void FinishGuiFrame()
   {
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
   }

   void DrawFrameData()
   {
      settings_->stop_watch.DrawGroupData("Frame Data");

      ImGui::Begin("Frame Data");

      ImGui::Text("Target FPS %0.2f", settings_->target_frames_per_second);
      ImGui::Text("Max Frame Delay: %0.2f", settings_->calculated_frame_delay);

      static int counterCPU = 0;
      static int delayCounterCPU = 0;
      static float CPUAverage = 0.0f;
      CPUAverage += settings_->stop_watch.GetTime("NanoUpdate");
      
      delayCounterCPU++;
      if (delayCounterCPU > 4)
      {
         delayCounterCPU -=4;

         plotCPUData[counterCPU] = CPUAverage / 4.0f;
         CPUAverage = 0.0f;
         counterCPU++;
         if (counterCPU >= plot_data_size)
            counterCPU = 0;
      }

      ImGui::Text("CPU Usage");
      ImGui::PlotLines("", plotCPUData, plot_data_size, 0, 0, 0, 8000, ImVec2(ImGui::GetColumnWidth(), 50));


      static int delayCounterMemory = 0;
      delayCounterMemory++;
      if (delayCounterMemory > 4)
      {
         delayCounterMemory -= 4;
         static int counterMemory = 0;
         counterMemory++;
         if (counterMemory >= plot_data_size)
            counterMemory = 0;
         plotMemoryData[counterMemory] = mem_usage.ReturnMemoryUsed() / 1000.0f;
      }
      ImGui::Text("Memory Usage");
      ImGui::PlotLines("",plotMemoryData, plot_data_size, 0, 0, 50000, 100000, ImVec2(ImGui::GetColumnWidth(), 50));

      ImGui::End();
   }

   void DrawGui()
   {
      DrawFrameData();
      ImGui::Begin("Debug Window");
      ImGui::Text("Screen Size: W-%i\tH-%i", settings_->screen_size.x, settings_->screen_size.y);
      IVec2 mousePos = InputManager::Instance()->MousePosition();
      ImGui::Text("MousePos: X: %i\tY:%i\n", mousePos.x, mousePos.y);
      ImGui::TextColored(mem_usage.IsMemoryMore() ? red : green, "Memory Used: %ikb",
                         mem_usage.ReturnMemoryUsed() / 1024);
      ImGui::Text("Selected Pixel Type: %s", paint_manager_->SelectedPixelName());
      ImGui::Text("Virtual Mouse: X-%i Y-%i", settings_->virtual_mouse.x, settings_->virtual_mouse.y);
      ImGui::Text("Simulated Pixels\nX: %i\tY:%i\nTotal:%i", Constant::chunk_size_x * Constant::world_size_x, Constant::chunk_size_y * Constant::world_size_y, Constant::chunk_total_size * Constant::world_size_x * Constant::world_size_y);
      ImGui::End();
   }

   ~GuiManager()
   {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplSDL2_Shutdown();
      ImGui::DestroyContext();
   }

private:
   PaintManager* paint_manager_{};
   GameSettings* settings_;
};
