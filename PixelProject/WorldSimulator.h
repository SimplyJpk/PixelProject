#pragma once

#include <boost/lockfree/queue.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/thread/win32/mutex.hpp>
#include <boost/asio/post.hpp>

#include <vector>
#include <iostream>
#include <unordered_map>

#include "PaintManager.h"
#include <SDL.h>
#include "Vec2.h"
#include "VecHash.h"
#include "GameObject.h"
#include "GameSettings.h"
#include "WorldChunk.h"
#include "WorldDataHandler.h"
#include "Camera.h"

#include <atomic>

#include "Constants.h"

#include <SDL_opengl.h>

#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextureUtility.h"
#include "WorldGenerator.h"

using namespace PixelProject;
using namespace boost;

class WorldSimulator final : public GameObject
{
   //TODO Need to make an update config for each type of terrain
public:
   
   const IVec2 world_dimensions = IVec2(Constant::world_size_x, Constant::world_size_y);

   std::atomic<int> thread_pool_tasks = 0;
   asio::thread_pool thread_pool{ 32 };
   const static int max_process_count = Constant::world_size_x * Constant::world_size_y;

   lockfree::queue<bool*> is_processed_queue{ max_process_count };

   // Might be worth looking into a different way to do this, this saves allocations but not sure if there is much benefit
   short* chunk_direction_order[static_cast<short>(E_PixelType::COUNT)];

   // XoshiroCpp::Xoroshiro128PlusPlus rng;
   // Most chunks that could be rendered at any time, we use this to quickly cull any impossible to render chunks
   IVec2 max_visible_chunks_on_screen = IVec2::Zero();

   Camera* cam;

   SDL_Rect world_render_rect;
   IVec2 max_render_box = IVec2::Zero();

   IVec2 pixel_world_dimensions;

   const int max_active_chunks = 12;

   GameSettings* game_settings;
   WorldDataHandler* world_data_handler;
   WorldGenerator* world_generator;
   PaintManager* paint_manager;

   //? SDL_Texture* world_texture = nullptr;

   bool DEBUG_DrawChunkLines = false;
   bool DEBUG_DropSand = false;
   int DEBUG_SandDropRate = 20;

   int DEBUG_PenSize = 5;
   bool DEBUG_PrintPixelData = false;

   u_long DEBUG_FrameCounter = 0;
   float DEBUG_ZoomLevel = 1;

   GLuint map_textures;
   unsigned int VBO;
   unsigned int VAO;
   unsigned int EBO;
   float vertices[20] = {
      // positions                    // texture coords
       0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
       0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
      -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
   };
   unsigned int indices[6] = {
       0, 1, 3, // first triangle
       1, 2, 3  // second triangle
   };

   WorldSimulator(GameSettings* settings, WorldGenerator* world_generator_)
   {
      //TODO Should have this be an offset from the camera?
      world_render_rect = {
         Constant::chunk_size_x, Constant::chunk_size_y, settings->screen_size.x, settings->screen_size.y
      };
      game_settings = settings;
      // We calculate the max number of visible chunks on screen
      //? Do we need to add 2? Is this enough? Is it to much?
      max_visible_chunks_on_screen = IVec2((game_settings->screen_size.x / Constant::chunk_size_x) + 2,
         (game_settings->screen_size.y / Constant::chunk_size_y) + 2);
      // Max pixel width/height allowed for the texture
      max_render_box = IVec2(game_settings->screen_size.x + (Constant::chunk_size_x * 2),
         game_settings->screen_size.y + (Constant::chunk_size_y * 2));

      world_data_handler = WorldDataHandler::Instance();
      world_generator = world_generator_;

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

      // position attribute
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      //x // color attribute
      //x glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
      //x glEnableVertexAttribArray(1);
      // texture coord attribute
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);

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
   void FixedUpdate() override;
   void UpdateInput();

   //x void SubscribeInputs(InputHandler* inputHandler) override;
   //x void UpdateInput(const int eventType, const SDL_Event* _event) override;

   bool Draw(Camera* camera) override;

protected:
   uint8_t GetDistanceToBorder(short x, short y, short direction);
   // Ugly, can we improve this?
   short x_dir_ = -1, y_dir_ = 1;

   bool is_processed_array_[max_process_count][Constant::chunk_total_size];
   boost::lockfree::queue<bool*> used_processed_queue{ max_process_count };

   short x_loop_from_to_[2][3] = {
      {0, Constant::chunk_size_x, 1},
      {Constant::chunk_size_x - 1, -1, -1}
   };
   short y_loop_from_to_[2][3] = {
   {0, Constant::chunk_size_y, 1},
      {Constant::chunk_size_y - 1, -1, -1}
   };


   short x_loop_from_to_dir_[5][2][3] = {
      // North
      {{0, Constant::chunk_size_x, 1}, {Constant::chunk_size_x - 1, -1, -1}},
      // East
      {{Constant::chunk_size_x - 1, Constant::chunk_size_x, 1}, {Constant::chunk_size_x - 1, Constant::chunk_size_x - 2, -1}},
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

   // Returns true if the Pixel reacts to the neighbouring pixels type. Fills return_pixels with new pixel types for the two pixels if pixels need to be changed.
   static int8_t CheckLogic(const int direction, BasePixel* pixel, const E_PixelType neighbour_type,
      E_PixelType* return_pixels);

   static bool DoesChunkHaveNeighbour(WorldChunk** neighbours, short direction);

private:
   void ClearWorld();
   void DebugShowChunkProcessPieces();
   void DebugDrawPixelRange();

   inline static Xoshiro256PlusPlus rng;
};
