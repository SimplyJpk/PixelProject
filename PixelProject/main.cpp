//TODO Clean this up a bit
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

#include <chrono>
#include <math.h>

//TODO Fix the really bad chunk tearing
//TODO Maybe fix up file paths, start using folders for sanity. https://stackoverflow.com/questions/3242897/can-i-configure-visual-studio-to-use-real-folders-instead-of-filters-in-c-proj
//TODO - > Check common standards for folders

using namespace PixelProject;

InputManager* input_manager;
WorldSimulator* world_sim;
GuiManager* gui_manager;

int main(int argc, char** argv)
{
		IO::create_file_if_not_exist("/data/");

		typedef std::chrono::steady_clock clock;
		typedef std::chrono::duration<float, std::milli > duration;

		auto frameStart = clock::now();
		auto frameEnd = clock::now();
		float frameOverflow = 0.0f;

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
		// End


		std::unique_ptr<GameSettings> settings = std::make_unique<GameSettings>();
		settings->LoadSettings(config);

		//TODO Init WorldData, need to improve this, pretty awkward having it here.
		WorldDataHandler::Instance()->AddPixelData(new SpacePixel());
		WorldDataHandler::Instance()->AddPixelData(new GroundPixel());
		WorldDataHandler::Instance()->AddPixelData(new SandPixel());
		WorldDataHandler::Instance()->AddPixelData(new WaterPixel());
		WorldDataHandler::Instance()->AddPixelData(new WoodPixel());
		WorldDataHandler::Instance()->AddPixelData(new OilPixel());
		WorldDataHandler::Instance()->AddPixelData(new FirePixel());

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
		bool debugFrameByFrameUpdate = false;

		stopWatch.StoreTime("Start");
		//! Start Finished
		// Update
		while (!input_manager->IsShuttingDown())
		{
				frameStart = std::chrono::steady_clock::now();
				//x stopWatch.UpdateTime("FrameTime");
				//x stopWatch.UpdateTime("Update");
				//x stopWatch.UpdateTime("Input");
				//
				// Check Inputs
				input_manager->Update();
				if (input_manager->IsShuttingDown()) break;
				stopWatch.StoreTime("Input", static_cast<duration>(clock::now() - frameStart).count());

				//TODO Move this somewhere better?
				settings->paint_manager->UpdateInput();
				world_sim->UpdateInput();

				if (input_manager->GetKeyButton(KeyCode::_0))
						debugFrameByFrameUpdate = !debugFrameByFrameUpdate;
				// Updates
				if (!debugFrameByFrameUpdate || input_manager->GetKeyDown(KeyCode::Down)) {
						world_sim->Update();
				}
				mainCam->Update();

				stopWatch.StoreTime("Update", static_cast<duration>(clock::now() - frameStart).count());

				if (SDL_GetTicks() - lastUpdate > 1000) {
						currentFps = frameCounter;
						frameCounter = 0;
						lastUpdate = SDL_GetTicks();
				}
				totalFrames++;
				frameCounter++;


				auto drawStart = clock::now();
				//x stopWatch.UpdateTime("Draw");
				//
				// Clear Screen
				SDL_RenderClear(renderer);

				world_sim->Draw(mainCam);

				//? Delete this? DEBUG
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderDrawLine(renderer, settings->virtual_mouse.x, settings->virtual_mouse.y, settings->virtual_mouse.x, settings->virtual_mouse.y);

				// Copy our texture
				//x SDL_RenderCopy(renderer, texture, NULL, &textureRect);
				FC_Draw(font, renderer, 10, 10, "Target FPS: %0.2f \nFrames in Last Second: %i\nFPS: %i", settings->target_frames_per_second, frameCounter, currentFps);

				// Draw GUI
				gui_manager->DrawGui();
				// Render
				SDL_RenderPresent(renderer);

				stopWatch.StoreTime("Draw", static_cast<duration>(clock::now() - drawStart).count());

        // Wait a bit

        frameEnd = clock::now();
        duration time =  frameEnd - frameStart;
        frameOverflow += time.count();

        stopWatch.StoreTime("FrameTime", time.count());

        // If our FPS is to high, we lose granular control over sleep, we move to a while loop wait, not ideal.
				// TODO Look into a better means?
				if (settings->calculated_frame_delay < 8.0f)
				{
						if (frameOverflow >= settings->calculated_frame_delay)
								frameOverflow = 0;

						while (time.count() < settings->calculated_frame_delay)
						{
								time = clock::now() - frameStart;
						}
				}
				else if (settings->calculated_frame_delay > frameOverflow) {
						//TODO Doing something wrong here, target FPS isn't being maintained
						const int sleep = static_cast<int>(settings->calculated_frame_delay - frameOverflow);
            frameOverflow = settings->calculated_frame_delay - (sleep + frameOverflow);
            SDL_Delay(sleep);
        }
				else {
						frameOverflow = 0.0f;
				}
				//? Debug
				// printf("%0.2f frameOverflow\n", frameOverflow);
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