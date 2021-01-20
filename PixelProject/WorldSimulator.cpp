#include "WorldSimulator.h"
#include <cmath>
#include <map>
#include "Camera.h"
#include <thread>
#include <vector>
#include <atomic>

#include "Logger.h"
#include "InputManager.h"

#include <math.h>

void WorldSimulator::Start()
{
   //gameSettings->Screen_Size = IVec2(WORLD_DIMENSIONS.x;
   // pixels = new Uint32[WORLD_DIMENSIONS.x, WORLD_DIMENSIONS.y, ChunkTotalSize];
   // isProcessed = new bool[WORLD_DIMENSIONS.x, WORLD_DIMENSIONS.y, ChunkTotalSize];

   // Generate our chunks and the pixel data
   for (int y = 0; y < world_dimensions.y; y++)
   {
      for (int x = 0; x < world_dimensions.x; x++)
      {
         IVec2 chunkIndex = IVec2(x, y);
         // Create and store our chunk
         WorldChunk* newChunk = new WorldChunk(chunkIndex);
         // Add chunk to map
         chunks.insert(std::make_pair(chunkIndex, newChunk));
         //x worldChunks.push_back(newChunk);
         // newChunk->pixels = new Uint32[chunk_total_size]{ 0 };
         is_chunk_processed[chunkIndex] = nullptr;
      }
   }
   // Loop through once more, but this time we build a list of neighbours for each chunk for quicker access
   for (int y = 0; y < world_dimensions.y; y++)
   {
      for (int x = 0; x < world_dimensions.x; x++)
      {
         // int _chunkIndex = (_y * WORLD_DIMENSIONS.x) + _x;
         WorldChunk* localChunk = chunks[IVec2(x, y)];

         // If North is free
         if (y > 0)
         {
            localChunk->SetNeighbour(North, chunks[IVec2(x, y - 1)]);
            // North West
            if (x > 0)
               localChunk->SetNeighbour(NorthWest, chunks[IVec2(x - 1, y - 1)]);
            // North East
            if (x < world_dimensions.x - 1)
               localChunk->SetNeighbour(NorthEast, chunks[IVec2(x + 1, y - 1)]);
         }
         // South
         if (y < world_dimensions.y - 1)
         {
            localChunk->SetNeighbour(South, chunks[IVec2(x, y + 1)]);
            // South West
            if (x > 0)
               localChunk->SetNeighbour(SouthWest, chunks[IVec2(x - 1, y + 1)]);
            // South East
            if (x < world_dimensions.x - 1)
               localChunk->SetNeighbour(SouthEast, chunks[IVec2(x + 1, y + 1)]);
         }
         // East West
         if (x > 0)
            localChunk->SetNeighbour(West, chunks[IVec2(x - 1, y)]);
         if (x < world_dimensions.x - 1)
            localChunk->SetNeighbour(East, chunks[IVec2(x + 1, y)]);
      }
   }

   for (size_t i = 0; i < 100; i++)
   {
      is_processed_queue.push(is_processed_array_[i]);
   }

   // Create our world texture, we use this to render the world chunks.
   world_texture = SDL_CreateTexture(game_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                     game_settings->screen_size.x + (Constant::chunk_size_x * 2),
                                     game_settings->screen_size.y + (Constant::chunk_size_y * 2));
   if (world_texture == nullptr)
   {
      printf("WorldTexture failed to Init\nError:%s\n", SDL_GetError());
      SDL_ClearError();
   }
}

void WorldSimulator::Pen(const IVec2& point, BasePixel* pixel_type, const int size)
{
   // Grab our CameraPosition in Pixels
   const SDL_Rect cameraPos = cam->view_port;
   // Calculate the Position of the Camera in the world in Chunks
   const Vec2 cameraWorldPosition = Vec2(static_cast<float>(cameraPos.x) / Constant::chunk_size_x,
                                         static_cast<float>(cameraPos.y) / Constant::chunk_size_y);
   // Same as above, but now we round to floor
   const IVec2 cameraChunk = IVec2(cameraWorldPosition.x, cameraWorldPosition.y);
   // Now we remove our position to get our offset
   const IVec2 cameraWorldOffset = IVec2((cameraWorldPosition.x - cameraChunk.x) * Constant::chunk_size_x,
                                         (cameraWorldPosition.y - cameraChunk.y) * Constant::chunk_size_y);

   const short max = pixel_type->colour_count;

   const IVec2 newPoint = IVec2(DEBUG_ZoomLevel * point.x, DEBUG_ZoomLevel * point.y);

   int Left = point.x - size;
   int Right = Left + size * 2;
   int Top = point.y - size;
   int Bottom = Top + size * 2;
   float radius = powf(size, 2);
   for (int y = Top; y <= Bottom; ++y)
   {
      if (y < 0 || y >= world_dimensions.y * Constant::chunk_size_y)
         continue;
      const int yFloor = floor(y / Constant::chunk_size_y);
      if (yFloor > world_dimensions.y) continue;

      for (int x = Left; x <= Right; ++x)
      {
         if (x < 0 || x >= world_dimensions.x * Constant::chunk_size_x)
            continue;
         const int xFloor = floor(x / Constant::chunk_size_x);
         if (xFloor > world_dimensions.x) continue;

         double dist = powf(point.x - x, 2.0) + powf(point.y - y, 2.0);
         if (dist <= radius)
         {
            chunks[IVec2(xFloor + cameraChunk.x, yFloor + cameraChunk.y)]->pixels[
               ((y - (yFloor * Constant::chunk_size_y) - cameraWorldOffset.y) * Constant::chunk_size_x) + (x - (xFloor *
                     Constant::chunk_size_x) -
                  cameraWorldOffset.x)
            ] = pixel_type->type_colours[(rng() % max)];
         }
      }
   }
}

void WorldSimulator::Update()
{
   //TODO Remove this
   DEBUG_FrameCounter++;
   if (DEBUG_DropSand)
   {
      BasePixel* sandPixel = world_data_handler->GetPixelFromIndex(3);
      const short maxSandColours = sandPixel->colour_count;
      for (int x = 0; x < world_dimensions.x; x++)
      {
         for (int xDim = 0; xDim < Constant::chunk_size_x; xDim++)
         {
            if (rng() % DEBUG_SandDropRate == 0)
            {
               chunks[IVec2(x, 0)]->pixels[(xDim * 0) + xDim] = sandPixel->type_colours[0, (rng() % maxSandColours)];
            }
         }
      }
   }
   //TODO Remove above

   int xStage, yStage;
   int chunkUpdates = 0;

   x_dir_ = (x_dir_ == 0 ? 1 : 0); // rng() % 2;
   y_dir_ = 1;

   for (int x = 0; x < world_dimensions.x; x++)
   {
      for (int y = 0; y < world_dimensions.y; y++)
      {
         WorldChunk* accessedChunk = chunks[IVec2(x, y)];
         bool* processedBools;
         is_processed_queue.pop(processedBools);
         is_chunk_processed[accessedChunk->position] = processedBools;
         used_processed_queue.push(processedBools);
      }
   }

   // Update the world in a checker pattern
   for (auto i = 0; i < 4; i++)
   {
      switch (i)
      {
      case 0: xStage = 0;
         yStage = 0;
         break;
      case 1: xStage = 1;
         yStage = 1;
         break;
      case 2: xStage = 1;
         yStage = 0;
         break;
      case 3: xStage = 0;
         yStage = 1;
         break;
      }

      // We fetch an array that we can use without allocations
      auto* const chunkDirectionOrder = chunk_direction_order_containers[0];
      // And fill it with Update Orders for each pixel type. This is only called once per chunk per frame.
      world_data_handler->FillWithPixelUpdateOrders(chunkDirectionOrder);

      for (auto xChunk = xStage; xChunk < world_dimensions.x; xChunk += 2)
      {
         if (xChunk >= world_dimensions.x)
            continue;
         for (auto yChunk = yStage; yChunk < world_dimensions.y; yChunk += 2)
         {
            if (yChunk < 0)
               continue;

            ++thread_pool_tasks;

            // Start
            const auto chunkIndex = IVec2(xChunk, yChunk);

            // Submit a lambda object to the pool.
            post(thread_pool, [this, chunkDirectionOrder, chunkIndex, chunkUpdates]() mutable
            {
               //Now we know our chunk indexes we create a local group to simplify lookup
               auto* localPixels = chunks[chunkIndex]->pixels; // [_localChunkIndex] ->pixels;
               Uint32* neighbourPixels = localPixels; // [_localChunkIndex] ->pixels;

               bool* isProcessed = is_chunk_processed[chunkIndex];
               bool* neighbourIsProcessed = isProcessed;

               E_PixelType returnPixels[2] = {E_PixelType::UNDEFINED, E_PixelType::UNDEFINED};

               WorldChunk** neighbourChunks = chunks[chunkIndex]->neighbour_chunks;

               //TODO Work out the best way to organize this
               int pieceOrder[] =
               {
                  (x_dir_ == 0 ? 2 : 1),
                  4,
                  0,
                  3,
                  (x_dir_ == 0 ? 1 : 2),
               };

               for (auto piece = 0; piece < sizeof(pieceOrder) / sizeof(pieceOrder[0]); piece++)
               {
                  if (pieceOrder[piece] == 4)
                  {
                     neighbourPixels = localPixels;
                     neighbourIsProcessed = isProcessed;
                  }
                  // printf("Piece: %i, xStart: %i, xTo: %i, xDir:%i\tyStart: %i, yTo: %i, yDir:%i\n", piece, x_loop_attempt[piece][x_dir_][From], x_loop_attempt[piece][x_dir_][To], x_loop_attempt[piece][x_dir_][Dir], y_loop_attempt[piece]//[!x_ dir_][From], y_loop_attempt[piece][x_dir_][To], y_loop_attempt[piece][x_dir_][Dir]);
                  /// <summary>
                  /// Our Inner Chunk Update
                  /// </summary>
                  for (auto
                       x = x_loop_from_to_dir_[pieceOrder[piece]][x_dir_][From];
                       x != x_loop_from_to_dir_[pieceOrder[piece]][x_dir_][To];
                       x += x_loop_from_to_dir_[pieceOrder[piece]][x_dir_][Dir])
                  {
                     for (auto
                          y = y_loop_from_to_dir_[pieceOrder[piece]][y_dir_][From];
                          y != y_loop_from_to_dir_[pieceOrder[piece]][y_dir_][To];
                          y += y_loop_from_to_dir_[pieceOrder[piece]][y_dir_][Dir])
                     {
                        const short localIndex = (y * Constant::chunk_size_x) + x;
                        // If the pixel is empty, or something beat us to update it
                        if (localPixels[localIndex] == 0 || isProcessed[localIndex]) continue;

                        auto* pixel = world_data_handler->GetPixelFromPixelColour(localPixels[localIndex]);

                        const short* pixelDirOrder = chunkDirectionOrder[pixel->pixel_index];
                        for (auto directionIndex = 0; directionIndex < static_cast<short>(DIR_COUNT); directionIndex++)
                        {
                           int direction = pixelDirOrder[directionIndex];
                           // If Direction is DIR_COUNT all other values will be DIR_COUNT and can be safely aborted.
                           if (direction == DIR_COUNT) break;

                           short neighbourIndex;
                           if (pieceOrder[piece] == 4)
                           {
                              neighbourIndex = GetInnerNeighbourIndex(localIndex, direction);
                           }
                           else
                           {
                              if (GetOuterNeighbourIndex(localIndex, y, x, direction, neighbourIndex))
                              {
                                 //TODO Any way to work this out in advance? vv
                                 if (neighbourChunks[direction] == nullptr)
                                    continue;
                                 neighbourPixels = neighbourChunks[direction]->pixels;
                                 neighbourIsProcessed = is_chunk_processed[neighbourChunks[direction]->position];
                              }
                              else
                              {
                                 neighbourPixels = localPixels;
                                 neighbourIsProcessed = isProcessed;
                              }
                           }

                           auto* pixelNeighbour = world_data_handler->GetPixelFromPixelColour(
                              neighbourPixels[neighbourIndex]);
                           const auto neighbourType = pixelNeighbour->GetType();

                           // Now we ask the Pixel what it wants to do with its neighbour
                           if (!CheckLogic(pixelDirOrder[directionIndex], pixel, neighbourType, returnPixels)) continue;

                           if (DEBUG_PrintPixelData)
                           {
                              printf("P-%i\t%i->%i\tX: %i\tY:%i\tUPDATE:%i\t%i\t->\t%i\n", pieceOrder[piece],
                                     pixelDirOrder[directionIndex], direction, x, y, DEBUG_FrameCounter, localIndex,
                                     neighbourIndex);
                           }

                           if (returnPixels[0] != E_PixelType::UNDEFINED)
                           {
                              if (returnPixels[0] != E_PixelType::UNDEFINED)
                              {
                                 localPixels[localIndex] = world_data_handler->GetPixelFromType(returnPixels[0])->
                                                                               GetRandomColour();
                                 isProcessed[localIndex] = true;
                                 returnPixels[0] = E_PixelType::UNDEFINED;
                              }
                              if (returnPixels[1] != E_PixelType::UNDEFINED)
                              {
                                 neighbourPixels[neighbourIndex] = world_data_handler->GetPixelFromType(returnPixels[1])
                                    ->GetRandomColour();
                                 neighbourIsProcessed[neighbourIndex] = true;
                                 returnPixels[1] = E_PixelType::UNDEFINED;
                              }
                           }
                           else
                           {
                              std::swap(localPixels[localIndex], neighbourPixels[neighbourIndex]);
                              neighbourIsProcessed[neighbourIndex] = true;
                           }

                           // ProcessLogicResults(world_data_handler, returnPixels, localPixels[localIndex], neighbourPixels[neighbourIndex]);
                           break;
                        }
                     }
                  }
               }
               --thread_pool_tasks;
            });
            chunkUpdates++;
         }
      }
      while (thread_pool_tasks > 0)
      {
         // Sleep(1);
      }
   }

   while (!used_processed_queue.empty())
   {
      bool* data;
      used_processed_queue.pop(data);
      memset(data, false, Constant::chunk_total_size);
      is_processed_queue.push(data);
   }
   //TODO Improve this?
   for (auto y = 0; y < world_dimensions.y; y++)
   {
      for (auto x = 0; x < world_dimensions.x; x++)
      {
         auto const chunkIndex = IVec2(x, y);
         is_chunk_processed[chunkIndex] = nullptr;
      }
   }
}

inline short WorldSimulator::GetInnerNeighbourIndex(const short local, const int direction)
{
   switch (direction)
   {
   case North:
      return local - Constant::chunk_size_x;
   case NorthEast:
      return local - Constant::chunk_size_x + 1;
   case East:
      return local + 1;
   case SouthEast:
      return local + Constant::chunk_size_x + 1;
   case South:
      return local + Constant::chunk_size_x;
   case SouthWest:
      return local + Constant::chunk_size_x - 1;
   case West:
      return local - 1;
   case NorthWest:
      return local - Constant::chunk_size_x - 1;
   default:
      return -1;
   }
}

inline bool WorldSimulator::DoesChunkHaveNeighbour(WorldChunk** neighbours, const short direction)
{
   return neighbours[direction] != nullptr;
}

inline bool WorldSimulator::GetOuterNeighbourIndex(const short local, const short y, const short x, int& direction,
                                                   short& neighbour_index)
{
   //TODO This could probably be made to target the more likely conditionals first to save some cpu time.

   // Above each case will be the potential paths for a pixel to move that takes it outside the current chunk.
   // If these states aren't met, we pass into the GetInnerNeighbourIndex as it is known safe.
   switch (direction)
   {
      // North
   case North:
      if (y == 0)
      {
         neighbour_index = Constant::chunk_total_size - Constant::chunk_size_x + x;
         return true;
      }
      break;


      // NorthEast, North, East
   case NorthEast:
      if (y == 0)
      {
         // NorthEast
         if (x == Constant::chunk_size_x - 1)
         {
            neighbour_index = Constant::chunk_total_size - Constant::chunk_size_x;
            return true;
         }
         // North
         if (x < Constant::chunk_size_x - 1)
         {
            neighbour_index = Constant::chunk_total_size - Constant::chunk_size_x + x + 1;
            direction = North;
            return true;
         }
      }
      // East
      if (x == Constant::chunk_size_x - 1 && y > 0)
      {
         neighbour_index = local - Constant::chunk_size_x - x;
         direction = East;
         return true;
      }
      break;


      // East
   case East:
      // East
      if (x == Constant::chunk_size_x - 1)
      {
         neighbour_index = local - x;
         return true;
      }
      break;


      // SouthEast, East, South
   case SouthEast:
      if (y == Constant::chunk_size_y - 1)
      {
         // South East (Corner)
         if (x == Constant::chunk_size_x - 1)
         {
            neighbour_index = 0;
            return true;
         }
         // South
         if (x < Constant::chunk_size_x - 1)
         {
            neighbour_index = x + 1;
            direction = South;
            return true;
         }
      }
      // East
      if (x == Constant::chunk_size_x - 1 && y < Constant::chunk_size_y - 1)
      {
         neighbour_index = local + 1;
         direction = East;
         return true;
      }
      break;


      // South
   case South:
      // South
      if (y == Constant::chunk_size_y - 1)
      {
         neighbour_index = x;
         return true;
      }
      break;


      // SouthWest, South, West
   case SouthWest:
      if (y == Constant::chunk_size_y - 1)
      {
         // South West
         if (x == 0)
         {
            neighbour_index = Constant::chunk_size_x - 1;
            return true;
         }
         // South
         if (x > 0)
         {
            neighbour_index = x - 1;
            direction = South;
            return true;
         }
      }
      // West
      if (x == 0)
      {
         neighbour_index = local + (Constant::chunk_size_x * 2) - 1;
         direction = West;
         return true;
      }
      break;


      // West
   case West:
      // West
      if (x == 0)
      {
         neighbour_index = local + Constant::chunk_size_x - 1;
         return true;
      }
      break;


      // NorthWest, North, West
   case NorthWest:
      if (x == 0)
      {
         // NorthWest
         if (y == 0)
         {
            neighbour_index = Constant::chunk_total_size - 1;
            return true;
         }
         // West
         if (y > 0)
         {
            neighbour_index = local - 1;
            direction = West;
            return true;
         }
      }
      // North
      if (x > 0 && y == 0)
      {
         neighbour_index = Constant::chunk_total_size - 1 - Constant::chunk_size_x + x;
         direction = North;
         return true;
      }
      break;
   }
   neighbour_index = GetInnerNeighbourIndex(local, direction);
   return false;
}

inline bool WorldSimulator::CheckLogic(const int direction, BasePixel* pixel, const E_PixelType neighbour_type,
                                       E_PixelType* return_pixels)
{
   switch (direction)
   {
   case North:
      return pixel->NorthLogic(neighbour_type, return_pixels);
   case NorthEast:
      return pixel->NorthEastLogic(neighbour_type, return_pixels);
   case East:
      return pixel->EastLogic(neighbour_type, return_pixels);
   case SouthEast:
      return pixel->SouthEastLogic(neighbour_type, return_pixels);
   case South:
      return pixel->SouthLogic(neighbour_type, return_pixels);
   case SouthWest:
      return pixel->SouthWestLogic(neighbour_type, return_pixels);
   case West:
      return pixel->WestLogic(neighbour_type, return_pixels);
   case NorthWest:
      return pixel->NorthWestLogic(neighbour_type, return_pixels);
   default:
      return false;
   }
}


void WorldSimulator::UpdateInput()
{
   InputManager* input = InputManager::Instance();
   const IVec2 mousePos = input->MousePosition();

   if (input->GetMouseButton(MouseLeft))
   {
      Pen(mousePos, game_settings->paint_manager->selected_pixel, DEBUG_PenSize);
   }
   if (input->GetMouseButton(MouseRight))
   {
      Pen(mousePos, world_data_handler->GetPixelFromIndex(0), DEBUG_PenSize);
   }

   if (input->GetKeyDown(KeyCode::J))
   {
      DEBUG_PenSize++;
   }
   if (input->GetKeyDown(KeyCode::K))
   {
      DEBUG_PenSize--;
      if (DEBUG_PenSize <= 0)
      {
         DEBUG_PenSize = 1;
      }
   }

   if (input->GetKeyDown(KeyCode::D))
   {
      DEBUG_DrawChunkLines = !DEBUG_DrawChunkLines;
   }
   if (input->GetKeyDown(KeyCode::Space))
   {
      DEBUG_DropSand = !DEBUG_DropSand;
   }
   if (input->GetKeyDown(KeyCode::M))
   {
      DEBUG_SandDropRate += 5;
   }
   if (input->GetKeyDown(KeyCode::N))
   {
      if (DEBUG_SandDropRate > 1)
      {
         DEBUG_SandDropRate--;
      }
   }
   if (input->GetKeyDown(KeyCode::Up))
   {
      DEBUG_PrintPixelData = !DEBUG_PrintPixelData;
   }
   if (input->GetKeyDown(KeyCode::Period))
   {
      Pen(mousePos, world_data_handler->GetPixelFromType(E_PixelType::Water), 1);
   }
   if (input->GetKeyDown(KeyCode::Comma))
   {
      Pen(game_settings->virtual_mouse, world_data_handler->GetPixelFromType(E_PixelType::Water), 1);
   }
   //TODO Editing Settings, bad
   if (input->GetKeyDown(KeyCode::F))
   {
      game_settings->virtual_mouse.x -= 1 * (input->GetKeyButton(KeyCode::LShift) ? 5 : 1);
   }
   if (input->GetKeyDown(KeyCode::H))
   {
      game_settings->virtual_mouse.x += 1 * (input->GetKeyButton(KeyCode::LShift) ? 5 : 1);
   }
   if (input->GetKeyDown(KeyCode::T))
   {
      game_settings->virtual_mouse.y -= 1 * (input->GetKeyButton(KeyCode::LShift) ? 5 : 1);
   }
   if (input->GetKeyDown(KeyCode::G))
   {
      game_settings->virtual_mouse.y += 1 * (input->GetKeyButton(KeyCode::LShift) ? 5 : 1);
   }

   if (input->GetKeyDown(KeyCode::E))
   {
      DEBUG_ZoomLevel += 0.01f;
   }
   else if (input->GetKeyDown(KeyCode::R))
   {
      DEBUG_ZoomLevel += -0.01f;
   }

   if (input->GetKeyDown((KeyCode::S)))
   {
      for (int x = 0; x < world_dimensions.x; x++)
      {
         for (int y = 0; y < world_dimensions.y; y++)
         {
            chunks[IVec2(x, y)]->StartSave(chunks[IVec2(x, y)]->FilePath().c_str());
         }
      }
   }
   else if (input->GetKeyDown((KeyCode::L)))
   {
      for (int x = 0; x < world_dimensions.x; x++)
      {
         for (int y = 0; y < world_dimensions.y; y++)
         {
            chunks[IVec2(x, y)]->StartLoad(chunks[IVec2(x, y)]->FilePath().c_str());
         }
      }
   }

   //TODO Move this into its own method?
   //? Only debug, a more complete solution that only deletes pixels of type would be cool
   if (input->GetKeyDown(KeyCode::Return))
   {
      ClearWorld();
   }
}

void WorldSimulator::ClearWorld()
{
   for (int y = 0; y < world_dimensions.y; y++)
   {
      for (int x = 0; x < world_dimensions.x; x++)
      {
         memset(chunks[IVec2(x, y)]->pixels, 0, Constant::chunk_total_size * sizeof(Uint32));
      }
   }
}

bool WorldSimulator::Draw(Camera* camera)
{
   // Copy our texture
   SDL_Rect rect;
   rect.w = Constant::chunk_size_x;
   rect.h = Constant::chunk_size_y;

   // Grab our CameraPosition in Pixels
   const SDL_Rect cameraPos = camera->view_port;
   // Calculate the Position of the Camera in the world in Chunks
   const Vec2 cameraWorldPosition = Vec2(static_cast<float>(cameraPos.x) / Constant::chunk_size_x,
                                         static_cast<float>(cameraPos.y) / Constant::chunk_size_y);
   // Same as above, but now we round to floor
   const IVec2 cameraChunk = IVec2(cameraWorldPosition.x, cameraWorldPosition.y);
   // Now we remove our position to get our offset
   IVec2 cameraWorldOffset = IVec2((cameraWorldPosition.x - cameraChunk.x) * Constant::chunk_size_x,
                                   (cameraWorldPosition.y - cameraChunk.y) * Constant::chunk_size_y);
   //? printf("CameraWorld X: %f, Y: %f\n", cameraWorldPosition.x, cameraWorldPosition.y);
   //? printf("CameraPixelOffset X: %i, Y: %i\n", cameraWorldOffset.x, cameraWorldOffset.y);

   const IVec2 screenSize = game_settings->screen_size;

   //TODO Fix all this vv
   for (int xVal = 0; xVal < max_visible_chunks_on_screen.x; xVal++)
   {
      const int xChunk = xVal; // +cameraChunk.x;
      if (xChunk < 0) continue;
      if (xChunk >= world_dimensions.x)
      {
         xVal = max_visible_chunks_on_screen.x;
         continue;
      }

      for (int yVal = 0; yVal < max_visible_chunks_on_screen.y; yVal++)
      {
         const int yChunk = yVal; // cameraChunk.y;
         if (yChunk < 0) continue;
         if (yChunk >= world_dimensions.y)
         {
            yVal = max_visible_chunks_on_screen.y;
            continue;
         }

         // We add one just to center the world based on how we're rendering (Bad choice)
         rect.x = ((xVal + 1) * Constant::chunk_size_x); // +cameraWorldOffset.x;
         rect.y = ((yVal + 1) * Constant::chunk_size_y); // +cameraWorldOffset.y;

         // If part of our visible chunk isn't on screen we need to update the texture a different way
         if (rect.x + Constant::chunk_size_x > max_render_box.x || rect.y + Constant::chunk_size_y > max_render_box.y)
         {
            //printf("We Skipped X:%i Y:%i\n", XChunk, y + cameraChunk.y);
         }
         else
         {
            // We update the texture using the entire chunk data
            if (SDL_UpdateTexture(world_texture, &rect, chunks[IVec2(xChunk, yChunk)]->pixels,
                                  Constant::chunk_size_x * sizeof(Uint32)))
            {
            }
         }
      }
   }

   //TODO Replace this with a better zoom
   SDL_Rect zoomrect = world_render_rect;
   zoomrect.w *= DEBUG_ZoomLevel;
   zoomrect.h *= DEBUG_ZoomLevel;

   if (SDL_RenderCopy(game_renderer, world_texture, &zoomrect, nullptr) != 0)
   {
      printf("WorldRender Failed!\nError:%s\n", SDL_GetError());
      SDL_ClearError();
   }

   //TODO Move or remove this
   SDL_SetRenderDrawColor(game_renderer, 50, 50, 50, 50);
   if (DEBUG_DrawChunkLines)
   {
      const IVec2 maxGridSize((screenSize.x + (Constant::chunk_size_x * 2)) / Constant::chunk_size_x,
                              (screenSize.y + (Constant::chunk_size_y * 2)) / Constant::chunk_size_y);
      for (int x = 1; x < maxGridSize.x; x++)
      {
         SDL_RenderDrawLine(game_renderer, (x * Constant::chunk_size_x) / DEBUG_ZoomLevel, 0,
                            (x * Constant::chunk_size_x) / DEBUG_ZoomLevel,
                            (Constant::chunk_size_y * maxGridSize.y - 1));
      }
      for (int y = 1; y < maxGridSize.y; y++)
      {
         SDL_RenderDrawLine(game_renderer, 0, (y * Constant::chunk_size_y) / DEBUG_ZoomLevel,
                            (Constant::chunk_size_x * maxGridSize.x) - 1,
                            (y * Constant::chunk_size_y) / DEBUG_ZoomLevel);
      }
   }
   return true;
}

void WorldSimulator::DebugShowChunkProcessPieces()
{
   Uint32 pieceOrderTestColour[] = {
      0xff000000, // Red
      0x0000ff00, // Blue
      0xffff0000, // Yellow
      0x00ff0000, // Green
      0xffffff00, // White
      0xcc00cc00 // Purple
   };

   int pieceOrder[] =
   {
      4,
      0,
      1,
      2,
      3,
   };


   ClearWorld();
   for (auto piece = 0; piece < sizeof(pieceOrder) / sizeof(pieceOrder[0]); piece++)
   {
      for (auto xChunk = 0; xChunk < world_dimensions.x; xChunk++)
      {
         for (auto yChunk = 0; yChunk < world_dimensions.y; yChunk++)
         {
            const auto chunkIndex = IVec2(xChunk, yChunk);
            for (auto
                 y = y_loop_from_to_dir_[pieceOrder[piece]][y_dir_][From];
                 y != y_loop_from_to_dir_[pieceOrder[piece]][y_dir_][To];
                 y += y_loop_from_to_dir_[pieceOrder[piece]][y_dir_][Dir])
            {
               for (auto
                    x = x_loop_from_to_dir_[pieceOrder[piece]][x_dir_][From];
                    x != x_loop_from_to_dir_[pieceOrder[piece]][x_dir_][To];
                    x += x_loop_from_to_dir_[pieceOrder[piece]][x_dir_][Dir])
               {
                  auto* localPixels = chunks[chunkIndex]->pixels;
                  const short localIndex = (y * Constant::chunk_size_x) + x;
                  localPixels[localIndex] = pieceOrderTestColour[piece];
               }
            }
         }
      }
   }
}
