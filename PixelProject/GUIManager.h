#pragma once
// ImGUI
#include "imgui_sdl.h"
#include "imgui.h"
#include "GameSettings.h"
#include "MemoryUsage.h"

class GUIManager
{
public:
		MemoryUsage memUsage;
		//Temp Colours
		ImVec4 red = ImVec4(1, 0, 0, 1);
		ImVec4 green = ImVec4(0, 1, 0, 1);

		GUIManager(SDL_Renderer* renderer, GameSettings* _settings) {
				settings = _settings;

				ImGui::CreateContext();
				ImGuiSDL::Initialize(renderer, _settings->Screen_Size.x, _settings->Screen_Size.y);
		}

		void DrawFrameData() {
				ImGui::Begin("Frame Data");
				ImGui::SetWindowSize(ImVec2(240, 240));
				ImGui::SetWindowPos(ImVec2(settings->Screen_Size.x - 245, 275));
				ImGui::Text(settings->_stopWatch->GetData().c_str());
				ImGui::End();
		}

		void DrawGUI() {
				ImGui::NewFrame();
				DrawFrameData();
				ImGui::Begin("Debug Window");
				ImGui::SetWindowSize(ImVec2(240, 240));
				ImGui::SetWindowPos(ImVec2(settings->Screen_Size.x - 245, 15));
				ImGui::Text("Screen Size: W-%i\tH-%i", settings->Screen_Size.x, settings->Screen_Size.y);
				ImGui::TextColored(memUsage.isMemoryMore() ? red : green, "Memory Used: %ikb", memUsage.ReturnMemoryUsed() / 1024);
				ImGui::Text("Selected Pixel Type: %s", settings->_paintManager->SelectedPixelName());
				ImGui::End();
				ImGui::Render();
				ImGuiSDL::Render(ImGui::GetDrawData());
		}
private:
		GameSettings* settings;
};

