#include <SDL.h>
#include <iostream>
#include <ctime>
#include "SDL_FontCache/SDL_FontCache.h"
#include "GameSettings.h"

#include "ConfigFile.h"

// My stuff
#include "Vec2.h"
#include "WorldSimulator.h"
#include "Camera.h"
#include "GUIManager.h"

#include "IO.h"

#include "InputManager.h"

#include "PixelTypeIncludes.h"
#include "DebugStopWatch.h"

//TODO Fix the really bad chunk tearing
//TODO Maybe fix up file paths, start using folders for sanity. https://stackoverflow.com/questions/3242897/can-i-configure-visual-studio-to-use-real-folders-instead-of-filters-in-c-proj
//TODO - > Check common standards for folders

using namespace PixelProject;

InputManager* input_manager;
WorldSimulator* world_sim;
GuiManager* gui_manager;

int main(int argc, char** argv)
{
		printf("Loading config file..\n");
		ConfigFile config;
		config.StartLoad(config.FilePath().c_str(), true);

		DebugStopWatch stopWatch;
		stopWatch.SetTimerState("Update", true);
		//! General Initialization
		stopWatch.AddTimer("Init", false);
		stopWatch.AddTimer("Start", false);
		stopWatch.AddTimer("Update", true);
		stopWatch.AddTimer("Draw", true);
		stopWatch.AddTimer("FrameTime", true);
		stopWatch.AddTimer("Input", false);

		// In-Game Commands
		printf("/--\t\tCommands\n");
		printf("|- TAB\t\t- Change Pixel type\n");
		printf("|- Mouse1\t- Draw Selected Pixel\n");
		printf("|- Mouse2\t- Clear 5x5 Pixels\n");
		printf("|- J\t\t- +1 PenSize\n");
		printf("|- K\t\t- -1 PenSize\n");
		printf("|- N\t\t- Increase Sand Spawn\n");
		printf("|- M\t\t- Slow Sand Spawn\n");
		printf("|- D\t\t- Hide Chunk Borders\n");
		printf("|- SPACE\t- Toggle Sand Spawn\n");
		printf("|- ENTER\t- Restart World (Clears Everything)\n\n");
		printf("|- 0\t\t- Toggle FrameByFrame Update\n");
		printf("|--> Down Arrow\t- Progress by 1 Frame\n");
		printf("|--> Up Arrow\t- Toggle Pixel Data Print\n");
		printf("|- Esc\t\t- Close Game\n\n");
		// End


		auto settings = std::make_unique<GameSettings>();
		settings->LoadSettings(config);

		//TODO Init WorldData, need to improve this, pretty awkward having it here.
		WorldDataHandler::Instance()->AddPixelData(new SpacePixel());
		WorldDataHandler::Instance()->AddPixelData(new GroundPixel());
		WorldDataHandler::Instance()->AddPixelData(new SandPixel());
		WorldDataHandler::Instance()->AddPixelData(new WaterPixel());

		// Initalize our settings
		settings->paint_manager = new	PaintManager();
		settings->screen_size = IVec2(settings->screen_size.x, settings->screen_size.y);
		settings->stop_watch = &stopWatch;

		// Create a new Window to use
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
		SDL_Window* window = SDL_CreateWindow("Pixel Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, settings->screen_size.x, settings->screen_size.y, 0);

		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		// Init ImGUI
		gui_manager = new GuiManager(renderer, settings.get());

		FC_Font* font = FC_CreateFont();
		FC_LoadFont(font, renderer, "fonts/FreeSans.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
		settings->font = font;

		Uint32 lastUpdate = 0;

		Uint32 frameCounter = 0;
		Uint32 totalFrames = 0;
		Uint32 currentFps = 0;

		lastUpdate = SDL_GetTicks();

		// Initialize all our junk
		input_manager = InputManager::Instance();
		// Create our WorldSimulator
		world_sim = new WorldSimulator(renderer, settings.get());
		Camera* mainCam = new Camera(IVec2(0, 0), settings.get());

		// Pass world simulator an instance of Camera
		world_sim->cam = mainCam;

		stopWatch.StoreTime("Init");
		//! General Initialization of Engine Complete

		//! Start
		stopWatch.UpdateTime("Start");

		world_sim->Start();
		mainCam->Start();

		//TODO REMOVE THIS
		bool debugFramebyframeUpdate = true;

		stopWatch.StoreTime("Start");
		//! Start Finished
		// Update
		while (!input_manager->IsShuttingDown())
		{
				stopWatch.UpdateTime("FrameTime");
				stopWatch.UpdateTime("Update");

				totalFrames++;
				frameCounter++;
				const Uint32 frameStart = SDL_GetTicks();

				stopWatch.UpdateTime("Input");
				// Check Inputs
				input_manager->Update();
				if (input_manager->IsShuttingDown()) break;
				stopWatch.StoreTime("Input");

				//TODO Move this somewhere better?
				settings->paint_manager->UpdateInput();
				world_sim->UpdateInput();

				if (input_manager->GetKeyButton(KeyCode::_0))
						debugFramebyframeUpdate = !debugFramebyframeUpdate;
				// Updates
				if (!debugFramebyframeUpdate || input_manager->GetKeyDown(KeyCode::Down)) {
						world_sim->Update();
				}
				mainCam->Update();

				stopWatch.StoreTime("Update");

				if (SDL_GetTicks() - lastUpdate > 1000) {
						currentFps = frameCounter;
						frameCounter = 0;
						lastUpdate = SDL_GetTicks();
				}

				stopWatch.UpdateTime("Draw");
				// Clear Screen
				SDL_RenderClear(renderer);

				world_sim->Draw(mainCam);

				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderDrawLine(renderer, settings->virtual_mouse.x, settings->virtual_mouse.y, settings->virtual_mouse.x, settings->virtual_mouse.y);
				// Copy our texture
				//x SDL_RenderCopy(renderer, texture, NULL, &textureRect);
				FC_Draw(font, renderer, 10, 10, "Target FPS: %i \nFrames in Last Second: %i\nFPS: %i",
				        settings->target_frames_per_second, frameCounter, currentFps);

				// Draw GUI
				gui_manager->DrawGui();
				// Render
				SDL_RenderPresent(renderer);

				stopWatch.StoreTime("Draw");

				stopWatch.StoreTime("FrameTime");
				// Wait a bit
				const int frameTime = SDL_GetTicks() - frameStart;
				if (settings->calculated_frame_delay > frameTime) {
						SDL_Delay(settings->calculated_frame_delay - frameTime);
				}

		}
		// Free all our resources
		FC_FreeFont(font);
		ImGuiSDL::Deinitialize();
		// delete[] pixels;
		// SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);

		SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
}