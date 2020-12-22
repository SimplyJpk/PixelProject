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
#include "WorldDataHandler.h"

#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost\thread\win32\mutex.hpp>

#include "SDL_FontCache/SDL_FontCache.h"

#include "WorldRules.h"

class WorldSimulator : public GameObject
{
		//TODO Need to make an update config for each type of terrain
public:
		//TODO Add to settings of some sort?
		const IVec2 WORLD_DIMENSIONS = IVec2(WORLD_SIZE_X, WORLD_SIZE_Y);

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
		WorldDataHandler* worldDataHandler;

		SDL_Texture* worldTexture = nullptr;

		bool DEBUG_DrawChunkLines = true;
		bool DEBUG_DropSand = true;
		int DEBUG_SandDropRate = 20;

		int DEBUG_PenSize = 5;

		//std::vector<SDL_Texture*> activeTextures;

		WorldSimulator(SDL_Renderer* renderer, GameSettings* settings) {
				//TODO SHould have this be an offset from the camera?
				WorldRenderRect = { CHUNK_SIZE_X, CHUNK_SIZE_Y, settings->Screen_Size.x, settings->Screen_Size.y };
				gameRenderer = renderer;
				gameSettings = settings;
				// We calculate the max number of visible chunks on screen
				//? Do we need to add 2? Is this enough? Is it to much?
				MaxVisibleChunksOnScreen = IVec2((gameSettings->Screen_Size.x / CHUNK_SIZE_X) + 2, (gameSettings->Screen_Size.y / CHUNK_SIZE_Y) + 2);
				// Max pixel width/height allowed for the texture
				MaxRenderBox = IVec2(gameSettings->Screen_Size.x + (CHUNK_SIZE_X * 2), gameSettings->Screen_Size.y + (CHUNK_SIZE_Y * 2));

				worldDataHandler = WorldDataHandler::Instance();
		}

		//TODO Chunk object?
		std::unordered_map<IVec2, WorldChunk*> chunks;
		// std::vector<WorldChunk*> worldChunks;

		// std::vector<Uint32*> pixels;
		// std::vector<bool*> isProcessed;

		void Pen(IVec2 point, BasePixel* pixelType, int size = 5);

		void Start() override;
		void Update() override;

		void UpdateInput();
		//x void SubscribeInputs(InputHandler* inputHandler) override;
		//x void UpdateInput(const int eventType, const SDL_Event* _event) override;

		bool Draw(Camera* camera) override;

protected:
		// Attempts to update the entire chunk using the game logic.
		//? void UpdateChunk(int x, int y, int isProcessedIndex);
		// Returns the index to the chunk array, should be used for neighbouring calls to save repeat calls.
		Uint32* returnChunk(int chunkIndex);
};