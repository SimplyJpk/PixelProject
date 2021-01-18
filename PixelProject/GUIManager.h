#pragma once
// ImGUI
#include "imgui_sdl.h"
#include "imgui.h"
#include "GameSettings.h"
#include "MemoryUsage.h"

class GuiManager
{
public:
   MemoryUsage mem_usage;
   //Temp Colours
   ImVec4 red = ImVec4(1, 0, 0, 1);
   ImVec4 green = ImVec4(0, 1, 0, 1);

   GuiManager(SDL_Renderer* renderer, GameSettings* settings)
   {
      settings_ = settings;

      ImGui::CreateContext();
      ImGuiSDL::Initialize(renderer, settings->screen_size.x, settings->screen_size.y);
   }

   void DrawFrameData() const
   {
      ImGui::Begin("Frame Data");
      ImGui::SetWindowSize(ImVec2(240, 240));
      ImGui::SetWindowPos(ImVec2(settings_->screen_size.x - 245, 275));
      ImGui::Text(settings_->stop_watch->GetData().c_str());
      ImGui::Text("Target FPS %0.2f", settings_->target_frames_per_second);
      ImGui::Text("Max Frame Delay: %0.2f", settings_->calculated_frame_delay);
      ImGui::End();
   }

   void DrawGui()
   {
      ImGui::NewFrame();
      DrawFrameData();
      ImGui::Begin("Debug Window");
      ImGui::SetWindowSize(ImVec2(240, 240));
      ImGui::SetWindowPos(ImVec2(settings_->screen_size.x - 245, 15));
      ImGui::Text("Screen Size: W-%i\tH-%i", settings_->screen_size.x, settings_->screen_size.y);
      ImGui::TextColored(mem_usage.IsMemoryMore() ? red : green, "Memory Used: %ikb",
                         mem_usage.ReturnMemoryUsed() / 1024);
      ImGui::Text("Selected Pixel Type: %s", settings_->paint_manager->SelectedPixelName());
      ImGui::Text("Virtual Mouse: X-%i Y-%i", settings_->virtual_mouse.x, settings_->virtual_mouse.y);
      ImGui::End();
      ImGui::Render();
      ImGuiSDL::Render(ImGui::GetDrawData());
   }

private:
   GameSettings* settings_;
};
