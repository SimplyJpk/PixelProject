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
#include <boost/thread/win32/mutex.hpp>

#include "SDL_FontCache/SDL_FontCache.h"

#include "WorldRules.h"

#include <ctime>
#include "./lib/XoshiroCpp.hpp"

class WorldSimulator final : public GameObject
{
		//TODO Need to make an update config for each type of terrain
public:
		//TODO Add to settings of some sort?
		const IVec2 world_dimensions = IVec2(WORLD_SIZE_X, WORLD_SIZE_Y);

		std::atomic<int> thread_pool_tasks = 0;
		boost::asio::thread_pool thread_pool{32};
		const static int max_process_count = 100;
		bool is_processed_queue[max_process_count][CHUNK_SIZE_X * CHUNK_SIZE_Y];

		// Might be worth looking into a different way to do this, this saves allocations but not sure if there is much benefit
		short* chunk_direction_order_containers[max_process_count][static_cast<short>(E_PixelType::COUNT)];

		XoshiroCpp::Xoshiro256PlusPlus rng{ (time(nullptr)) };
		// Most chunks that could be rendered at any time, we use this to quickly cull any impossible to render chunks
		IVec2 max_visible_chunks_on_screen = IVec2::Zero();
		
		Camera* cam;

		SDL_Rect world_render_rect;
		IVec2 max_render_box = IVec2::Zero();

		Uint32 chunk_total_size = 0;

		IVec2 pixel_world_dimensions;

		SDL_Renderer* game_renderer;
		const int max_active_chunks = 12;

		GameSettings* game_settings;
		WorldDataHandler* world_data_handler;

		SDL_Texture* world_texture = nullptr;

		bool DEBUG_DrawChunkLines = true;
		bool DEBUG_DropSand = false;
		int DEBUG_SandDropRate = 20;

		int DEBUG_PenSize = 1;
		bool DEBUG_PrintPixelData = true;

		u_long DEBUG_FrameCounter = 0;
		float DEBUG_ZoomLevel = 1.0f;

		//std::vector<SDL_Texture*> activeTextures;

		WorldSimulator(SDL_Renderer* renderer, GameSettings* settings) {
				//TODO Should have this be an offset from the camera?
				world_render_rect = { CHUNK_SIZE_X, CHUNK_SIZE_Y, settings->screen_size.x, settings->screen_size.y };
				game_renderer = renderer;
				game_settings = settings;
				// We calculate the max number of visible chunks on screen
				//? Do we need to add 2? Is this enough? Is it to much?
				max_visible_chunks_on_screen = IVec2((game_settings->screen_size.x / CHUNK_SIZE_X) + 2, (game_settings->screen_size.y / CHUNK_SIZE_Y) + 2);
				// Max pixel width/height allowed for the texture
				max_render_box = IVec2(game_settings->screen_size.x + (CHUNK_SIZE_X * 2), game_settings->screen_size.y + (CHUNK_SIZE_Y * 2));

				world_data_handler = WorldDataHandler::Instance();
		}

		//TODO Chunk object?
		std::unordered_map<IVec2, WorldChunk*> chunks;
		// std::vector<WorldChunk*> worldChunks;

		// std::vector<Uint32*> pixels;
		// std::vector<bool*> isProcessed;

		void Pen(const IVec2& point, BasePixel* pixel_type, int size = 5);

		void Start() override;
		void Update() override;
		void UpdateInput();

		//x void SubscribeInputs(InputHandler* inputHandler) override;
		//x void UpdateInput(const int eventType, const SDL_Event* _event) override;

		bool Draw(Camera* camera) override;

protected:
		void GetStartAndToForLoop(const short& side, short& x_start, short& x_to, short& y_start, short& y_to) const;
		// Ugly, can we improve this?
		short x_dir_ = -1, y_dir_ = 1;
};