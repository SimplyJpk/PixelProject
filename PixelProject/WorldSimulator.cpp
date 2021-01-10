#include "WorldSimulator.h"
#include <cmath>
#include <map>
#include "Camera.h"
#include <thread>
#include <vector>
#include <atomic>

#include "Logger.h"
#include "ChunkUtility.h"
#include "InputManager.h"

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
   world_texture = SDL_CreateTexture(game_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, game_settings->screen_size.x + (CHUNK_SIZE_X * 2), game_settings->screen_size.y + (CHUNK_SIZE_Y * 2));
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
   const Vec2 cameraWorldPosition = Vec2(static_cast<float>(cameraPos.x) / CHUNK_SIZE_X,
      static_cast<float>(cameraPos.y) / CHUNK_SIZE_Y);
   // Same as above, but now we round to floor
   const IVec2 cameraChunk = IVec2(cameraWorldPosition.x, cameraWorldPosition.y);
   // Now we remove our position to get our offset
   const IVec2 cameraWorldOffset = IVec2((cameraWorldPosition.x - cameraChunk.x) * CHUNK_SIZE_X,
      (cameraWorldPosition.y - cameraChunk.y) * CHUNK_SIZE_Y);

   const short max = pixel_type->colour_count;

   const IVec2 newPoint = IVec2(DEBUG_ZoomLevel * point.x, DEBUG_ZoomLevel * point.y);

   //TODO A lot of math, we should be able to simplify this easily enough by storing within scope. 
   //TODO IE: (WORLD_DIM * CHUNK_DIM) This'll likely never change?
   if (newPoint.x >= 0 && newPoint.x < world_dimensions.x * CHUNK_SIZE_X)
   {
      if (newPoint.y >= 0 && newPoint.y <= world_dimensions.y * CHUNK_SIZE_Y)
      {
         for (int x = newPoint.x - size; x < point.x + size; x++)
         {
            // We skip calculations if our position is outside the realm of our world.
            if (x < 0 || x > world_dimensions.x * CHUNK_SIZE_X - 1) continue;
            // We get the floor of X divided by Size of Chunks
            static int xFloor;
            xFloor = floor(x / CHUNK_SIZE_X);
            if (xFloor > world_dimensions.x) continue;
            for (int y = newPoint.y - size; y < newPoint.y + size; y++)
            {
               if (y < 0 || y > world_dimensions.y * CHUNK_SIZE_Y - 1) continue;
               static int yFloor;
               yFloor = floor(y / CHUNK_SIZE_Y);
               if (yFloor > world_dimensions.y) continue;

               chunks[IVec2(xFloor + cameraChunk.x, yFloor + cameraChunk.y)]->pixels[
                  ((y - (yFloor * CHUNK_SIZE_Y) - cameraWorldOffset.y) * CHUNK_SIZE_X) + (x - (xFloor * CHUNK_SIZE_X) -
                     cameraWorldOffset.x)
               ] = pixel_type->type_colours[(rng() % max)];

                  if (size == 1) return;
            }
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
         for (int xDim = 0; xDim < CHUNK_SIZE_X; xDim++)
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

   x_dir_ = rng() % 1;
   y_dir_ = rng() % 1;
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

      for (auto xChunk = xStage; xChunk < world_dimensions.x; xChunk += 2)
      {
         if (xChunk >= world_dimensions.x)
            continue;
         for (auto yChunk = yStage; yChunk < world_dimensions.y; yChunk += 2)
         {
            if (yChunk >= world_dimensions.y)
               continue;

            ++thread_pool_tasks;

            // Submit a lambda object to the pool.
            post(thread_pool, [this, xChunk, yChunk, chunkUpdates]() mutable {

               // Start
               const IVec2 chunkIndex = IVec2(xChunk, yChunk);

               //Now we know our chunk indexes we create a local group to simplify lookup
               Uint32* localPixels = chunks[chunkIndex]->pixels; // [_localChunkIndex] ->pixels;

               bool* isProcessed;
               is_processed_queue.pop(isProcessed);
               // Check if we've already 
               if (is_chunk_processed[chunkIndex] == nullptr) is_chunk_processed[chunkIndex] = isProcessed;

               used_processed_queue.push(isProcessed);

               E_PixelType returnPixels[2]{ E_PixelType::Space };

               // We fetch an array that we can use without allocations
               auto *const chunkDirectionOrder = chunk_direction_order_containers[chunkUpdates];
               // And fill it with Update Orders for each pixel type. This is only called once per chunk per frame.
               world_data_handler->FillWithPixelUpdateOrders(chunkDirectionOrder);

               for (auto 
                  y = y_loop_start_to_[y_dir_][From];
                  y != y_loop_start_to_[y_dir_][To];
                  y += y_loop_start_to_[y_dir_][Dir])
               {
                  for (auto 
                     x = x_loop_start_to_[x_dir_][From];
                     x != x_loop_start_to_[x_dir_][To];
                     x += x_loop_start_to_[x_dir_][Dir])
                  {
                     const short localIndex = (y * CHUNK_SIZE_X) + x;
                     // If the pixel is empty, or something beat us to update it
                     if (localPixels[localIndex] == 0 || isProcessed[localIndex]) continue;

                     BasePixel* pixel = world_data_handler->GetPixelFromPixelColour(localPixels[localIndex]);
                     const auto type = pixel->GetType();

                     const short* pixelDirOrder = chunkDirectionOrder[pixel->pixel_index];
                     for (int directionIndex = 0; directionIndex < static_cast<short>(DIR_COUNT); directionIndex++)
                     {
                        const int direction = pixelDirOrder[directionIndex];
                        if (direction == DIR_COUNT) continue;

                        const auto neighbourIndex = GetInnerNeighbourIndex(localIndex, direction);

                        auto* pixelNeighbour = world_data_handler->GetPixelFromPixelColour(localPixels[neighbourIndex]);
                        const auto neighbourType = pixelNeighbour->GetType();

                        // Now we ask the Pixel what it wants to do with its neighbour
                        const auto result = CheckLogic(direction, pixel, neighbourType, returnPixels);
                        if (result)
                        {
                           if (DEBUG_PrintPixelData) { printf("X: %i\tY:%i\tUPDATE:%i\t(IN)\n", x, y, DEBUG_FrameCounter); }
                           if (returnPixels[0] != returnPixels[1])
                           {
                              if (type != returnPixels[0])
                              {
                                 chunks[chunkIndex]->pixels[localIndex] = world_data_handler->GetPixelFromType(returnPixels[0])
                                    ->GetRandomColour();
                              }
                              if (neighbourType != returnPixels[1])
                              {
                                 chunks[chunkIndex]->pixels[localIndex] = world_data_handler->GetPixelFromType(returnPixels[1])
                                    ->GetRandomColour();
                              }
                           }
                           else
                           {
                              std::swap(chunks[chunkIndex]->pixels[localIndex], chunks[chunkIndex]->pixels[neighbourIndex]);
                           }
                           isProcessed[neighbourIndex] = true;
                           break;
                        }
                     }
                  }
               }

               //? We could hold onto our 8 neighbouring chunks pixels here, and almost brute force this with switch statements? WOuld be super ugly..
               //? Not sure how else to do all this..

               //! WorldChunk** neighbourChunks = chunks[chunkIndex]->neighbour_chunks;
               //! for (short shellUpdate = 0; shellUpdate < 4; shellUpdate++)
               //! {
               //!    GetStartAndToForLoop(shellUpdate, xStart, xTo, yStart, yTo);
               //!    for (short y = yStart; y != yTo; y += y_dir_)
               //!    {
               //!       for (short x = xStart; x != xTo; x += x_dir_)
               //!       {
               //!          const short localIndex = (y * CHUNK_SIZE_X) + x;
               //! 
               //!          if (!(localPixels[localIndex] != 0 && !isProcessed[localIndex]))
               //!          {
               //!             continue;
               //!          }
               //! 
               //!          BasePixel* pixel = world_data_handler->GetPixelFromPixelColour(localPixels[localIndex]);
               //!          const E_PixelType pixelType = pixel->GetType();
               //! 
               //!          const short* pixelDirOrder = chunkDirectionOrder[pixel->pixel_index];
               //!          for (int directionIndex = 0; directionIndex < static_cast<short>(DIR_COUNT); directionIndex++)
               //!          {
               //!             int dirIndex = pixelDirOrder[directionIndex];
               //!             if (dirIndex == DIR_COUNT) continue;
               //! 
               //!             bool isSameChunk = true;
               //! 
               //!             neighbour = nullptr;
               //!             switch (dirIndex)
               //!             {
               //!                // case ChunkDirection::NorthEast:
               //!                // 		if (x == CHUNK_SIZE_X - 1) {
               //!                // 				if (neighbourChunks[ChunkDirection::East] != nullptr) {
               //!                // 						neighbourIndex = (CHUNK_SIZE_X * (y)) - 1;
               //!                // 						neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[ChunkDirection::East]->pixels[neighbourIndex]);
               //!                // 						newChunkDir = ChunkDirection::East;
               //!                // 						isSameChunk = false;
               //!                // 						break;
               //!                // 				}
               //!                // 		}
               //!                // 		continue;
               //!                // case ChunkDirection::NorthWest:
               //!                // 		if (x == 0) {
               //!                // 				if (neighbourChunks[ChunkDirection::West] != nullptr) {
               //!                // 						neighbourIndex = (CHUNK_SIZE_X * (y)) - 1;
               //!                // 						neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[ChunkDirection::West]->pixels[neighbourIndex]);
               //!                // 						newChunkDir = ChunkDirection::West;
               //!                // 						isSameChunk = false;
               //!                // 						break;
               //!                // 				}
               //!                // 		}
               //!                // 		continue;
               //!                // case ChunkDirection::North:
               //!                // 		if (y == 0) {
               //!                // 				if (neighbourChunks[ChunkDirection::North] == nullptr) continue;
               //!                // 				neighbourIndex = ChunkTotalSize - x;
               //!                // 				neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[ChunkDirection::North]->pixels[neighbourIndex]);
               //!                // 				newChunkDir = ChunkDirection::North;
               //!                // 				isSameChunk = false;
               //!                // 		}
               //!                // 		else {
               //!                // 				neighbourIndex = _localIndex - CHUNK_SIZE_X;
               //!                // 		}
               //!                // 		break;
               //!                // case ChunkDirection::SouthEast:
               //!                // 		if (x == CHUNK_SIZE_X - 1) {
               //!                // 				if (neighbourChunks[ChunkDirection::East] != nullptr) {
               //!                // 						neighbourIndex = (CHUNK_SIZE_X * (y)) + 1;
               //!                // 						neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[ChunkDirection::East]->pixels[neighbourIndex]);
               //!                // 						newChunkDir = ChunkDirection::East;
               //!                // 						isSameChunk = false;
               //!                // 						break;
               //!                // 				}
               //!                // 		}
               //!                // case ChunkDirection::SouthWest:
               //!                // 		if (x == 0) {
               //!                // 				if (neighbourChunks[ChunkDirection::West] != nullptr) {
               //!                // 						neighbourIndex = (CHUNK_SIZE_X * (y)) - 1;
               //!                // 						neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[ChunkDirection::West]->pixels[neighbourIndex]);
               //!                // 						newChunkDir = ChunkDirection::West;
               //!                // 						isSameChunk = false;
               //!                // 						break;
               //!                // 				}
               //!                // 		}
               //!             case South:
               //!             {
               //!                if (y == CHUNK_SIZE_Y - 1)
               //!                {
               //!                   if (neighbourChunks[South] == nullptr) { continue; }
               //!                   neighbourIndex = x;
               //!                   dirIndex = South;
               //!                   neighbour = world_data_handler->GetPixelFromPixelColour(
               //!                      neighbourChunks[dirIndex]->pixels[neighbourIndex]);
               //!                }
               //!                else { neighbourIndex = localIndex + CHUNK_SIZE_X; }
               //!                break;
               //!             }
               //!             case West:
               //!             {
               //!                if (x == 0)
               //!                {
               //!                   if (neighbourChunks[West] == nullptr) { continue; }
               //!                   neighbourIndex = (CHUNK_SIZE_X * (y + 1)) - 1;
               //!                   dirIndex = West;
               //!                   neighbour = world_data_handler->GetPixelFromPixelColour(
               //!                      neighbourChunks[dirIndex]->pixels[neighbourIndex]);
               //!                }
               //!                else { neighbourIndex = localIndex - 1; }
               //!                break;
               //!             }
               //!             case East:
               //!             {
               //!                if (x == CHUNK_SIZE_X - 1)
               //!                {
               //!                   if (neighbourChunks[East] == nullptr) { continue; }
               //!                   neighbourIndex = (CHUNK_SIZE_X * (y)) + 1;
               //!                   dirIndex = East;
               //!                   neighbour = world_data_handler->GetPixelFromPixelColour(
               //!                      neighbourChunks[dirIndex]->pixels[neighbourIndex]);
               //!                }
               //!                else { neighbourIndex = localIndex + 1; }
               //!                break;
               //!             }
               //!             // case ChunkDirection::East:
               //!             // 		if (x == CHUNK_SIZE_X - 1) {
               //!             // 				if (neighbourChunks[ChunkDirection::East] != nullptr) {
               //!             // 						neighbourIndex = (CHUNK_SIZE_X * (y)) - 1;
               //!             // 						neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[ChunkDirection::East]->pixels[neighbourIndex]);
               //!             // 						newChunkDir = ChunkDirection::East;
               //!             // 						isSameChunk = false;
               //!             // 						break;
               //!             // 				}
               //!             // 		}
               //!             // 		continue;
               //!             default:
               //!                continue;
               //!             }
               //! 
               //!             if (neighbour == nullptr)
               //!                neighbour = world_data_handler->GetPixelFromPixelColour(localPixels[neighbourIndex]);
               //!             else
               //!                isSameChunk = false;
               //! 
               //!             neighbourType = neighbour->GetType();
               //! 
               //!             // Now we ask the Pixel what it wants to do with its neighbour
               //!             // this can return true/false and if the pixel needs to convert the neighbour to something returnPixels will have different values
               //!             bool result = false;
               //!             switch (dirIndex)
               //!             {
               //!             case North:
               //!                result = pixel->NorthLogic(neighbourType, returnPixels);
               //!                break;
               //!             case NorthEast:
               //!                result = pixel->NorthEastLogic(neighbourType, returnPixels);
               //!                break;
               //!             case East:
               //!                result = pixel->EastLogic(neighbourType, returnPixels);
               //!                break;
               //!             case SouthEast:
               //!                result = pixel->SouthEastLogic(neighbourType, returnPixels);
               //!                break;
               //!             case South:
               //!                result = pixel->SouthLogic(neighbourType, returnPixels);
               //!                break;
               //!             case SouthWest:
               //!                result = pixel->SouthWestLogic(neighbourType, returnPixels);
               //!                break;
               //!             case West:
               //!                result = pixel->WestLogic(neighbourType, returnPixels);
               //!                break;
               //!             case NorthWest:
               //!                result = pixel->NorthWestLogic(neighbourType, returnPixels);
               //!                break;
               //!             }
               //!             if (result)
               //!             {
               //!                if (DEBUG_PrintPixelData)
               //!                {
               //!                   printf("X: %i\tY:%i\tUPDATE:%i\t(OUT)\n", x, y, DEBUG_FrameCounter);
               //!                }
               //!                if (returnPixels[0] != returnPixels[1])
               //!                {
               //!                   if (pixelType != returnPixels[0])
               //!                   {
               //!                      chunks[chunkIndex]->pixels[localIndex] = world_data_handler->GetPixelFromType(
               //!                         returnPixels[0])->GetRandomColour();
               //!                   }
               //!                   if (neighbourType != returnPixels[1])
               //!                   {
               //!                      if (isSameChunk)
               //!                         chunks[chunkIndex]->pixels[localIndex] = world_data_handler->GetPixelFromType(
               //!                            returnPixels[1])->GetRandomColour();
               //!                      else
               //!                         neighbourChunks[dirIndex]->pixels[neighbourIndex] = world_data_handler->GetPixelFromType(
               //!                            returnPixels[1])->GetRandomColour();
               //!                   }
               //!                }
               //!                else
               //!                {
               //!                   if (isSameChunk)
               //!                      std::swap(chunks[chunkIndex]->pixels[localIndex],
               //!                         chunks[chunkIndex]->pixels[neighbourIndex]);
               //!                   else
               //!                      std::swap(chunks[chunkIndex]->pixels[localIndex],
               //!                         neighbourChunks[dirIndex]->pixels[neighbourIndex]);
               //!                }
               //!                if (isSameChunk) {
               //!                   isProcessed[neighbourIndex] = true;
               //!                }
               //!                else {
               //!                   WorldChunk* accessedChunk = neighbourChunks[dirIndex];
               //!                   bool* processedBools = is_chunk_processed[accessedChunk->position];
               //!                   if (processedBools == nullptr)
               //!                   {
               //!                      is_processed_queue.pop(processedBools);
               //!                      is_chunk_processed[accessedChunk->position] = processedBools;
               //!                      used_processed_queue.push(processedBools);
               //!                   }
               //!                   processedBools[neighbourIndex] = true;
               //!                }
               //!                break;
               //!             }
               //!          }
               //!       }
               //!    }
               //! }
               //End
               --thread_pool_tasks;
               });
            chunkUpdates++;
         }
      }
      while (thread_pool_tasks > 0)
      {
         Sleep(1);
      }
   }

   while (!used_processed_queue.empty())
   {
      bool* data;
      used_processed_queue.pop(data);
      memset(data, false, CHUNK_SIZE_X * CHUNK_SIZE_Y);
      is_processed_queue.push(data);
   }
   //TODO Improve this?
   for (int y = 0; y < world_dimensions.y; y++)
   {
      for (int x = 0; x < world_dimensions.x; x++)
      {
         IVec2 chunkIndex = IVec2(x, y);
         is_chunk_processed[chunkIndex] = nullptr;
      }
   }
   //TODO Can probbaly be deleted V
   //? //TODO look into doing this in 1 chunk/update. 
   //? for (int i = 0; i < chunkUpdates; i++)
   //? {
   //? 		memset(is_processed_array_[i], false, (CHUNK_SIZE_X * CHUNK_SIZE_Y));
   //? }
}

// Is this really helpful? or just in the way..
//TODO Clean this up a bit?
void WorldSimulator::GetStartAndToForLoop(const short& side, short& x_start, short& x_to, short& y_start, short& y_to) const
{
   switch (side)
   {
      // case ChunkDirection::North:
   case 0:
      y_start = 0;
      y_to = (y_dir_ == 1 ? 1 : -1);
      goto jumpToSetX;
      // case ChunkDirection::South:
   case 1:
      y_start = CHUNK_SIZE_Y - 1;
      y_to = (y_dir_ == 1 ? CHUNK_SIZE_Y : CHUNK_SIZE_Y - 2);
      // Set Common X
   jumpToSetX:
      x_start = (x_dir_ == 1 ? 0 : CHUNK_SIZE_X - 1);
      x_to = (x_dir_ == 1 ? CHUNK_SIZE_X - 1 : -1);
      break;

      // case ChunkDirection::East:
   case 2:
      x_start = 0;
      x_to = (x_dir_ == 1 ? 1 : -1);
      goto jumpToSetY;
      break;
      // case ChunkDirection::West:
   case 3:
      x_start = CHUNK_SIZE_X - 1;
      x_to = (x_dir_ == 1 ? CHUNK_SIZE_X : CHUNK_SIZE_X - 2);
      // Set Common Y
   jumpToSetY:
      y_start = (y_dir_ == 1 ? 0 : CHUNK_SIZE_Y - 1);
      y_to = (y_dir_ == 1 ? CHUNK_SIZE_Y - 1 : 0);
      break;
   }
}

inline short WorldSimulator::GetInnerNeighbourIndex(const short local, const int direction)
{
   switch (direction)
   {
   case North:
      return local - CHUNK_SIZE_X;
   case NorthEast:
      return local - CHUNK_SIZE_X + 1;
   case East:
      return local + 1;
   case SouthEast:
      return local + CHUNK_SIZE_X + 1;
   case South:
      return local + CHUNK_SIZE_X;
   case SouthWest:
      return local + CHUNK_SIZE_X - 1;
   case West:
      return local - 1;
   case NorthWest:
      return local - CHUNK_SIZE_X - 1;
   default: 
      return -1;
   }
}

inline bool WorldSimulator::CheckLogic(const int direction, BasePixel* pixel, const E_PixelType neighbour_type, E_PixelType* return_pixels)
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
      for (int y = 0; y < world_dimensions.y; y++)
      {
         for (int x = 0; x < world_dimensions.x; x++)
         {
            memset(chunks[IVec2(x, y)]->pixels, 0, chunk_total_size * sizeof(Uint32));
         }
      }
   }
}

bool WorldSimulator::Draw(Camera* camera)
{
   // Copy our texture
   SDL_Rect rect;
   rect.w = CHUNK_SIZE_X;
   rect.h = CHUNK_SIZE_Y;

   // Grab our CameraPosition in Pixels
   const SDL_Rect cameraPos = camera->view_port;
   // Calculate the Position of the Camera in the world in Chunks
   const Vec2 cameraWorldPosition = Vec2(static_cast<float>(cameraPos.x) / CHUNK_SIZE_X,
      static_cast<float>(cameraPos.y) / CHUNK_SIZE_Y);
   // Same as above, but now we round to floor
   const IVec2 cameraChunk = IVec2(cameraWorldPosition.x, cameraWorldPosition.y);
   // Now we remove our position to get our offset
   IVec2 cameraWorldOffset = IVec2((cameraWorldPosition.x - cameraChunk.x) * CHUNK_SIZE_X,
      (cameraWorldPosition.y - cameraChunk.y) * CHUNK_SIZE_Y);
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
         rect.x = ((xVal + 1) * CHUNK_SIZE_X); // +cameraWorldOffset.x;
         rect.y = ((yVal + 1) * CHUNK_SIZE_Y); // +cameraWorldOffset.y;

         // If part of our visible chunk isn't on screen we need to update the texture a different way
         if (rect.x + CHUNK_SIZE_X > max_render_box.x || rect.y + CHUNK_SIZE_Y > max_render_box.y)
         {
            //printf("We Skipped X:%i Y:%i\n", XChunk, y + cameraChunk.y);
         }
         else
         {
            // We update the texture using the entire chunk data
            if (SDL_UpdateTexture(world_texture, &rect, chunks[IVec2(xChunk, yChunk)]->pixels,
               CHUNK_SIZE_X * sizeof(Uint32)))
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
      const IVec2 maxGridSize((screenSize.x + (CHUNK_SIZE_X * 2)) / CHUNK_SIZE_X,
         (screenSize.y + (CHUNK_SIZE_Y * 2)) / CHUNK_SIZE_Y);
      for (int x = 1; x < maxGridSize.x; x++)
      {
         SDL_RenderDrawLine(game_renderer, (x * CHUNK_SIZE_X) / DEBUG_ZoomLevel, 0, (x * CHUNK_SIZE_X) / DEBUG_ZoomLevel,
            (CHUNK_SIZE_Y * maxGridSize.y - 1));
      }
      for (int y = 1; y < maxGridSize.y; y++)
      {
         SDL_RenderDrawLine(game_renderer, 0, (y * CHUNK_SIZE_Y) / DEBUG_ZoomLevel, (CHUNK_SIZE_X * maxGridSize.x) - 1,
            (y * CHUNK_SIZE_Y) / DEBUG_ZoomLevel);
      }
   }
   return true;
}
