#pragma once
#include <vector>
#include <iostream>
#include <unordered_map>

#include <SDL.h>
#include "Vec2.h"
#include "VecHash.h"
#include "GameObject.h"
#include "GameSettings.h"
#include "WorldChunk.h"

#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost\thread\win32\mutex.hpp>

#include "SDL_FontCache/SDL_FontCache.h"

class WorldSimulator : public GameObject
{
		//TODO Need to make an update config for each type of terrain

		//TODO make this better, maybe create an object for terrains
		Uint32 red = 0xFF000000;
		Uint32 green = 0x00FF0000;
		Uint32 blue = 0x0000FF00;
		Uint32 brown = 0x964b0000;
		Uint32 white = 0xFFFFFF00;
public:
		//TODO Add to settings of some sort?
		const IVec2 WORLD_DIMENSIONS = IVec2(4, 4);
		#define CHUNK_SIZE_X 128
		#define CHUNK_SIZE_Y 128

		std::atomic<int> threadPoolTasks = 0;
		boost::asio::thread_pool threadPool{32};
		const static int MaxProcessCount = 100;
		bool isProcessedQueue[MaxProcessCount][CHUNK_SIZE_X * CHUNK_SIZE_Y];

		// Most chunks that could be rendered at any time, we use this to quickly cull any impossible to render chunks
		IVec2 MaxVisibleChunksOnScreen = IVec2::Zero();
		
		Camera* cam;

		SDL_Rect WorldRenderRect;
		IVec2 MaxRenderBox = IVec2::Zero();

		Uint32 ChunkTotalSize = 0;

		IVec2 Pixel_World_Dimensions;

		SDL_Renderer* gameRenderer;
		const int MAX_ACTIVE_CHUNKS = 12;

		GameSettings* gameSettings;

		SDL_Texture* worldTexture = nullptr;

		//std::vector<SDL_Texture*> activeTextures;

		WorldSimulator(SDL_Renderer* renderer, GameSettings* settings) {
				//TODO SHould have this be an offset from the camera?
				WorldRenderRect = { CHUNK_SIZE_X, CHUNK_SIZE_Y, settings->_CONFIG_SCREEN_SIZE.x, settings->_CONFIG_SCREEN_SIZE.y };
				gameRenderer = renderer;
				gameSettings = settings;
				// We calculate the max number of visible chunks on screen
				//? Do we need to add 2? Is this enough? Is it to much?
				MaxVisibleChunksOnScreen = IVec2((gameSettings->_CONFIG_SCREEN_SIZE.x / CHUNK_SIZE_X) + 2, (gameSettings->_CONFIG_SCREEN_SIZE.y / CHUNK_SIZE_Y) + 2);
				// Max pixel width/height allowed for the texture
				MaxRenderBox = IVec2(gameSettings->_CONFIG_SCREEN_SIZE.x + (CHUNK_SIZE_X * 2), gameSettings->_CONFIG_SCREEN_SIZE.y + (CHUNK_SIZE_Y * 2));
		}

		//TODO Chunk object?
		std::unordered_map<IVec2, WorldChunk*> chunks;
		// std::vector<WorldChunk*> worldChunks;

		// std::vector<Uint32*> pixels;
		// std::vector<bool*> isProcessed;

		void Pen(IVec2 point, int size = 5);

		void Start() override;
		void Update() override;

		void SubscribeInputs(InputHandler* inputHandler) override;
		void UpdateInput(const int eventType, const SDL_Event* _event) override;

		bool Draw(Camera* camera) override;

protected:
		// Attempts to update the entire chunk using the game logic.
		void UpdateChunk(int x, int y, int isProcessedIndex);
		// Returns the index to the chunk array, should be used for neighbouring calls to save repeat calls.
		Uint32* returnChunk(int chunkIndex);
		// Moves a pixel from one location to another, returning true if anything was moved.
		bool MovePixel(Uint32* chunk, int from, int to, Uint32* toChunk = nullptr);
};