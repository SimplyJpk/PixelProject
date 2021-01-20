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

#include "Constants.h"

#include <ctime>
#include "./lib/XoshiroCpp.hpp"

#include <boost/lockfree/queue.hpp>
#include <queue>

using namespace PixelProject;

class WorldSimulator final : public GameObject
{
   //TODO Need to make an update config for each type of terrain
public:

   const IVec2 world_dimensions = IVec2(Constant::world_size_x, Constant::world_size_y);

   std::atomic<int> thread_pool_tasks = 0;
   boost::asio::thread_pool thread_pool{32};
   const static int max_process_count = 100;

   boost::lockfree::queue<bool*> is_processed_queue{max_process_count};

   // Might be worth looking into a different way to do this, this saves allocations but not sure if there is much benefit
   short* chunk_direction_order_containers[max_process_count][static_cast<short>(E_PixelType::COUNT)];

   XoshiroCpp::Xoshiro256PlusPlus rng{time(nullptr)};
   // Most chunks that could be rendered at any time, we use this to quickly cull any impossible to render chunks
   IVec2 max_visible_chunks_on_screen = IVec2::Zero();

   Camera* cam;

   SDL_Rect world_render_rect;
   IVec2 max_render_box = IVec2::Zero();

   IVec2 pixel_world_dimensions;

   SDL_Renderer* game_renderer;
   const int max_active_chunks = 12;

   GameSettings* game_settings;
   WorldDataHandler* world_data_handler;

   SDL_Texture* world_texture = nullptr;

   bool DEBUG_DrawChunkLines = false;
   bool DEBUG_DropSand = false;
   int DEBUG_SandDropRate = 20;

   int DEBUG_PenSize = 1;
   bool DEBUG_PrintPixelData = false;

   u_long DEBUG_FrameCounter = 0;
   float DEBUG_ZoomLevel = 1;

   //std::vector<SDL_Texture*> activeTextures;

   WorldSimulator(SDL_Renderer* renderer, GameSettings* settings)
   {
      //TODO Should have this be an offset from the camera?
      world_render_rect = {
         Constant::chunk_size_x, Constant::chunk_size_y, settings->screen_size.x, settings->screen_size.y
      };
      game_renderer = renderer;
      game_settings = settings;
      // We calculate the max number of visible chunks on screen
      //? Do we need to add 2? Is this enough? Is it to much?
      max_visible_chunks_on_screen = IVec2((game_settings->screen_size.x / Constant::chunk_size_x) + 2,
                                           (game_settings->screen_size.y / Constant::chunk_size_y) + 2);
      // Max pixel width/height allowed for the texture
      max_render_box = IVec2(game_settings->screen_size.x + (Constant::chunk_size_x * 2),
                             game_settings->screen_size.y + (Constant::chunk_size_y * 2));

      world_data_handler = WorldDataHandler::Instance();
   }

   //TODO Chunk object?
   std::unordered_map<IVec2, WorldChunk*> chunks;

   std::unordered_map<IVec2, bool*> is_chunk_processed;
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
   // Ugly, can we improve this?
   short x_dir_ = -1, y_dir_ = 1;

   bool is_processed_array_[max_process_count][Constant::chunk_total_size];
   boost::lockfree::queue<bool*> used_processed_queue{max_process_count};

   // TODO All seems to mostly work, now we need to work out how to organize these into Directional Bias preferences.. fun
   // TODO All seems to mostly work, now we need to work out how to organize these into Directional Bias preferences.. fun

   short x_loop_from_to_dir_[5][2][3] = {
      // North
      {{0, Constant::chunk_size_x, 1}, {Constant::chunk_size_x - 1, -1, -1}},
      // East
      {
         {Constant::chunk_size_x - 1, Constant::chunk_size_x, 1},
         {Constant::chunk_size_x - 1, Constant::chunk_size_x - 2, -1}
      },
      // West
      {{0, 1, 1}, {0, -1, -1}},
      // South
      {{0, Constant::chunk_size_x, 1}, {Constant::chunk_size_x - 1, -1, -1}},
      // Inner Chunk
      {{1, Constant::chunk_size_x - 1, 1}, {Constant::chunk_size_x - 2, 0, -1}}
   };

   // Only the Negatives are verified, need to make sure positives are accurate as well.
   short y_loop_from_to_dir_[5][2][3] = {
      // North
      {{0, 1, 1}, {0, -1, -1}},
      // East
      {{1, Constant::chunk_size_y - 1, 1}, {Constant::chunk_size_y - 2, 0, -1}},
      // West
      {{1, Constant::chunk_size_y - 1, 1}, {Constant::chunk_size_y - 2, 0, -1}},
      // South
      {
         {Constant::chunk_size_y - 1, Constant::chunk_size_y, 1},
         {Constant::chunk_size_y - 1, Constant::chunk_size_y - 2, -1}
      },
      // Inner Chunk
      {{1, Constant::chunk_size_y - 1, 1}, {Constant::chunk_size_y - 2, 0, -1}}
   };

   enum LoopHint
   {
      From,
      To,
      Dir
   };

   // Returns the index of the cell in the direction passed in.
   static short GetInnerNeighbourIndex(short local, int direction);
   // Returns the index of the cell in the neighbouring chunk of the index/direction passed in.
   static bool GetOuterNeighbourIndex(const short local, const short y, const short x, int& direction,
                                      short& neighbour_index);
   // Returns true if the Pixel reacts to the neighbouring pixels type. Fills return_pixels with new pixel types for the two pixels if pixels need to be changed.
   static bool CheckLogic(const int direction, BasePixel* pixel, const E_PixelType neighbour_type,
                          E_PixelType* return_pixels);

   static bool DoesChunkHaveNeighbour(WorldChunk** neighbours, short direction);
   static void ProcessLogicResults(WorldDataHandler* data_handler, const E_PixelType return_pixels[2],
                                   Uint32& from_pixel, Uint32& to_pixel);

private:
   void ClearWorld();
   void DebugShowChunkProcessPieces();
};
