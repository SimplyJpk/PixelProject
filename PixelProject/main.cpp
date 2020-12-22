#include <SDL.h>
#include <iostream>
#include <time.h>
#include "SDL_FontCache/SDL_FontCache.h"
#include "GameSettings.h"

// My stuff
#include "Vec2.h"
#include "WorldSimulator.h"
#include "Camera.h"
#include "GUIManager.h"

#include "InputManager.h"

#include "PixelTypeIncludes.h"

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
		// In-Game Commands
		printf("/--\t\tCommands\n");
		printf("|- TAB\t\t- Change Pixel type\n");
		printf("|- Mouse1\t- Draw Selected Pixel\n");
		printf("|- Mouse2\t- Clear 5x5 Pixels\n");
		printf("|- N\t\t- Increase Sand Spawn\n");
		printf("|- M\t\t- Slow Sand Spawn\n");
		printf("|- SPACE\t- Toggle Sand Spawn\n");
		printf("|- ENTER\t- Restart World (Clears Everything)\n");
		// End

		srand(time(NULL));

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

		// Create a new Window to use
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
		SDL_Window* window = SDL_CreateWindow("Pixel Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

		//TODO make this better, maybe create an object for terrains
		Uint32 red = 0xFF000000;
		Uint32 green = 0x00FF0000;
		Uint32 blue = 0x0000FF00;
		Uint32 brown = 0x964b0000;
		Uint32 white = 0xFFFFFF00;

		SDL_Event event;

		SDL_Renderer* renderer = NULL;
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		// Init ImGUI
		guiManager = new GUIManager(renderer, settings);

		// SDL_Texture* texture = SDL_CreateTexture(renderer,
		// 		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, CHUNK_DIMENSIONS.x, CHUNK_DIMENSIONS.y);
		// SDL_Rect textureRect;
		// textureRect.x = 0;
		// textureRect.y = 0;
		// textureRect.w = CHUNK_DIMENSIONS.x * 2;
		// textureRect.h = CHUNK_DIMENSIONS.y * 2;
		// 
		// Uint32* pixels = new Uint32[CHUNK_DIMENSIONS.x * CHUNK_DIMENSIONS.y];
		// bool* isProcessed = new bool[CHUNK_DIMENSIONS.x * CHUNK_DIMENSIONS.y];
		// 
		// // Set all pixels black
		// memset(pixels, 0, CHUNK_DIMENSIONS.x * CHUNK_DIMENSIONS.y * sizeof(Uint32));
		// // Clear bools
		// memset(isProcessed, false, CHUNK_DIMENSIONS.x * CHUNK_DIMENSIONS.y * sizeof(bool));

		FC_Font* font = FC_CreateFont();
		FC_LoadFont(font, renderer, "fonts/FreeSans.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
		settings->_font = font;

		Uint32 activeColour = blue;

		Uint32 frameStart;
		int frameTime;

		Uint32 lastUpdate = 0;

		Uint32 frameCounter = 0;
		Uint32 totalFrames = 0;
		Uint32 currentFPS = 0;

		int mouseX, mouseY;

		// Event
		bool leftMouseButtonDown = false;

		lastUpdate = SDL_GetTicks();

		// Initialize all our junk
		inputManager = InputManager::Instance();
		// Create our WorldSimulator
		worldSim = new WorldSimulator(renderer, settings);
		Camera* mainCam = new Camera(IVec2(0,0), settings);

		// Pass world simulator an instance of Camera
		worldSim->cam = mainCam;

		// Attach Events
		// inputHandler->Subscribe((InputEventMsg)ExitGame, InputEvent::USER_QUIT);

		//? Now in WorldSimulator
		//xinputHandler->Subscribe((InputEventMsg)PenDraw, InputEvent::USER_CLICK_DOWN);

		// Start
		worldSim->Start();
		mainCam->Start();
		// Update
		while (!Shutdown)
		{
				totalFrames++;
				frameCounter++;
				frameStart = SDL_GetTicks();

				// Check Inputs
				inputManager->Update();
				if (Shutdown) break;

				// Updates
				worldSim->Update();
				mainCam->Update();

				if (SDL_GetTicks() - lastUpdate > 1000) {
						currentFPS = frameCounter;
						frameCounter = 0;
						lastUpdate = SDL_GetTicks();
				}

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