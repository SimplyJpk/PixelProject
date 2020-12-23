#include <SDL.h>
#include <iostream>
#include <time.h>
#include "SDL_FontCache/SDL_FontCache.h"
#include "GameSettings.h"

#include "./lib/XoshiroCpp.hpp"

// My stuff
#include "Vec2.h"
#include "WorldSimulator.h"
#include "Camera.h"
#include "GUIManager.h"

#include "InputManager.h"

#include "PixelTypeIncludes.h"
#include "DebugStopWatch.h"


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720 - 32;

const int TARGET_FPS = 60;
const float frameDelay = (float)1000 / TARGET_FPS;

const int DRAW_SIZE = 5;

bool Shutdown = false;

InputManager* inputManager;
WorldSimulator* worldSim;
GUIManager* guiManager;

void ExitGame(int eventType, SDL_Event* _event) {
		Shutdown = true;
}

int main(int argc, char** argv)
{
		DebugStopWatch StopWatch;
		StopWatch.SetTimerState("Update", true);
		//! General Initialization
		StopWatch.AddTimer("Init", false);
		StopWatch.AddTimer("Start", false);
		StopWatch.AddTimer("Update", true);
		StopWatch.AddTimer("Draw", true);
		StopWatch.AddTimer("FrameTime", true);
		StopWatch.AddTimer("Input", false);

		// In-Game Commands
		printf("/--\t\tCommands\n");
		printf("|- TAB\t\t- Change Pixel type\n");
		printf("|- Mouse1\t- Draw Selected Pixel\n");
		printf("|- Mouse2\t- Clear 5x5 Pixels\n");
		printf("|- J\t\t- +1 PenSize\n");
		printf("|- K\t\t- -1 PenSize\n");
		printf("|- N\t\t- Increase Sand Spawn\n");
		printf("|- M\t\t- Slow Sand Spawn\n");
		printf("|- SPACE\t- Toggle Sand Spawn\n");
		printf("|- ENTER\t- Restart World (Clears Everything)\n");
		printf("|- Esc\t- Close Game\n");
		// End

		XoshiroCpp::Xoshiro256PlusPlus rng(time(NULL));

		// srand(time(NULL));

		

		//TODO Init WorldData, need to improve this, pretty awkward having it here.
		WorldDataHandler::Instance()->AddPixelData(new SpacePixel());
		WorldDataHandler::Instance()->AddPixelData(new GroundPixel());
		WorldDataHandler::Instance()->AddPixelData(new SandPixel());
		WorldDataHandler::Instance()->AddPixelData(new WaterPixel());

		// Initalize our settings
		//TODO Make a serializable settings file?
		//TODO Wrap this?
		GameSettings* settings = new GameSettings();
		settings->Screen_Size = IVec2(SCREEN_WIDTH, SCREEN_HEIGHT);
		settings->_stopWatch = &StopWatch;

		// Create a new Window to use
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
		SDL_Window* window = SDL_CreateWindow("Pixel Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

		SDL_Renderer* renderer = NULL;
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		// Init ImGUI
		guiManager = new GUIManager(renderer, settings);

		FC_Font* font = FC_CreateFont();
		FC_LoadFont(font, renderer, "fonts/FreeSans.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
		settings->_font = font;

		Uint32 frameStart;
		int frameTime;

		Uint32 lastUpdate = 0;

		Uint32 frameCounter = 0;
		Uint32 totalFrames = 0;
		Uint32 currentFPS = 0;

		lastUpdate = SDL_GetTicks();

		// Initialize all our junk
		inputManager = InputManager::Instance();
		// Create our WorldSimulator
		worldSim = new WorldSimulator(renderer, settings);
		Camera* mainCam = new Camera(IVec2(0,0), settings);

		// Pass world simulator an instance of Camera
		worldSim->cam = mainCam;

		StopWatch.StoreTime("Init");
		//! General Initialization of Engine Complete

		//! Start
		StopWatch.UpdateTime("Start");

		worldSim->Start();
		mainCam->Start();

		StopWatch.StoreTime("Start");
		//! Start Finished
		// Update
		while (!inputManager->IsShuttingDown())
		{
				StopWatch.UpdateTime("FrameTime");
				StopWatch.UpdateTime("Update");

				totalFrames++;
				frameCounter++;
				frameStart = SDL_GetTicks();

				StopWatch.UpdateTime("Input");
				// Check Inputs
				inputManager->Update();
				if (inputManager->IsShuttingDown()) break;
				StopWatch.StoreTime("Input");

				// Updates
				worldSim->Update();
				mainCam->Update();

				StopWatch.StoreTime("Update");

				if (SDL_GetTicks() - lastUpdate > 1000) {
						currentFPS = frameCounter;
						frameCounter = 0;
						lastUpdate = SDL_GetTicks();
				}

				StopWatch.UpdateTime("Draw");
				// Clear Screen
				SDL_RenderClear(renderer);

				worldSim->Draw(mainCam);
				// Copy our texture
				//x SDL_RenderCopy(renderer, texture, NULL, &textureRect);
				FC_Draw(font, renderer, 10, 10, "Target FPS: %i \nFrames in Last Second: %i\nFPS: %i", TARGET_FPS, frameCounter, currentFPS);

				// Draw GUI
				guiManager->DrawGUI();
				// Render
				SDL_RenderPresent(renderer);

				StopWatch.StoreTime("Draw");

				StopWatch.StoreTime("FrameTime");
				// Wait a bit
				frameTime = SDL_GetTicks() - frameStart;
				if (frameDelay > frameTime) {
						SDL_Delay(frameDelay - frameTime);
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