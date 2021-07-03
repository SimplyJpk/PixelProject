#include "WorldSimulator.h"

#define STB_IMAGE_IMPLEMENTATION
#include "PixelUtil.h"
#include "stb_image.h"

void WorldSimulator::Start()
{
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

   //? for (size_t i = 0; i < world_dimensions.x * world_dimensions.y; i++)
   //? {
   //?    is_processed_queue.push(is_processed_array_[i]);
   //? }

   //short yDim = Constant::world_size_y; // (game_settings->screen_size.y / Constant::chunk_size_y) + 2;
   //short xDim = Constant::world_size_x; // (game_settings->screen_size.x / Constant::chunk_size_x) + 2;

   // map_textures = new GLuint[yDim * xDim];
   glGenTextures(1, &map_textures);
   glBindTexture(GL_TEXTURE_2D, map_textures);

   TextureUtility::SetTexParams();

   glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, Constant::chunk_size_x, Constant::chunk_size_y, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, chunks[IVec2(0, 0)]->pixel_data);

   glUseProgram(game_settings->default_shader);
   glUniform1i(glGetUniformLocation(game_settings->default_shader, "ourTexture"), 0);

   for (int x = 0; x < world_dimensions.x; x++)
   {
      for (int y = 0; y < world_dimensions.y; y++)
      {
         world_generator->GenerateChunk(glm::vec2(x * Constant::chunk_size_x, y * Constant::chunk_size_y), chunks[IVec2(x, y)]);
      }
   }

   // Build Noise texture
   glActiveTexture(GL_TEXTURE1);
   glGenTextures(1, &map_noiseTexture);
   glBindTexture(GL_TEXTURE_2D, 1);
   
   map_noiseTextureData = new Uint8[Constant::chunk_total_size]{0};
   for (size_t i = 0; i < Constant::chunk_total_size; i++)
   {
      map_noiseTextureData[i] = rng() % 4;
   }
   glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, Constant::chunk_size_x, Constant::chunk_size_y, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, map_noiseTextureData);
   glUniform1i(glGetUniformLocation(game_settings->default_shader, "textureIndex"), 1);
}

void WorldSimulator::Pen(const IVec2& point, BasePixel* pixel_type, const int size, const bool override_pixels)
{
   // Grab our CameraPosition in Pixels
   // const SDL_Rect cameraPos = cam->view_port;
   // Calculate the Position of the Camera in the world in Chunks
   // const Vec2 cameraWorldPosition = Vec2(static_cast<float>(cameraPos.x) / Constant::chunk_size_x,
   //                                       static_cast<float>(cameraPos.y) / Constant::chunk_size_y);
   // Same as above, but now we round to floor
   // const IVec2 cameraChunk = IVec2(cameraWorldPosition.x, cameraWorldPosition.y);
   // // Now we remove our position to get our offset
   // const IVec2 cameraWorldOffset = IVec2((cameraWorldPosition.x - cameraChunk.x) * Constant::chunk_size_x,
   //                                       (cameraWorldPosition.y - cameraChunk.y) * Constant::chunk_size_y);

   const short max = pixel_type->colour_count;

   const IVec2 newPoint = IVec2(DEBUG_ZoomLevel * point.x, DEBUG_ZoomLevel * point.y);

   auto camPosition = cam->GetPosition();
   
   int XCenter = static_cast<int>(camPosition.x) + point.x;
   int YCenter = static_cast<int>(camPosition.y) + point.y;

   int Left = XCenter - size;
   int Right = XCenter + size;
   int Top =  YCenter - size;
   int Bottom = YCenter + size;
   float radius = powf(size, 2);

   const int WorldDimX = world_dimensions.x * Constant::chunk_size_x;
   const int WorldDimY = world_dimensions.y * Constant::chunk_size_y;

   for (int y = Top; y <= Bottom; ++y)
   {
      if (y < 0 || y >= WorldDimY)
         continue;
      const int yFloor = floor(y / Constant::chunk_size_y);
      if (yFloor > world_dimensions.y) continue;

      for (int x = Left; x <= Right; ++x)
      {
         if (x < 0 || x >= WorldDimX)
            continue;
         const int xFloor = floor(x / Constant::chunk_size_x);
         if (xFloor > world_dimensions.x) continue;

         const auto dist = powf(XCenter - x, 2.0) + powf(YCenter - y, 2.0);
         if (dist <= radius)
         {
            const Uint32 index = ((y - (yFloor * Constant::chunk_size_y)) * Constant::chunk_size_x) + (x - (xFloor * Constant::chunk_size_x));
            if (!override_pixels && chunks[IVec2(xFloor, yFloor)]->pixel_data[index] != static_cast<int>(E_PixelType::Space))
            {
               continue;
            }
            chunks[IVec2(xFloor, yFloor)]->pixel_data[index] = pixel_type->GetNewPixel();
         }
      }
   }
}

void WorldSimulator::Update()
{
   
}

void WorldSimulator::FixedUpdate()
{
   //? DebugDrawPixelRange();
   //? return;

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
               chunks[IVec2(x, 0)]->pixel_data[(xDim * 0) + xDim] = sandPixel->GetNewPixel();
            }
         }
      }
   }
   //TODO Remove above

   uint8_t xStage, yStage;
   int chunkUpdates = 0;

   x_dir_ = (x_dir_ == 0 ? 1 : 0); // rng() % 2;
   y_dir_ = 0;

   for (uint16_t x = 0; x < world_dimensions.x; x++)
   {
      for (uint16_t y = 0; y < world_dimensions.y; y++)
      {
         //? WorldChunk* accessedChunk = chunks[IVec2(x, y)];
         //? bool* processedBools;
         //? is_processed_queue.pop(processedBools);
         is_chunk_processed[chunks[IVec2(x, y)]->position] = is_processed_array_[chunkUpdates];
         chunkUpdates++;
         //? used_processed_queue.push(processedBools);
      }
   }
   chunkUpdates = 0;

   // Fill array with Update Orders for each pixel type. This is only called once per chunk per frame.
   world_data_handler->FillWithPixelUpdateOrders(chunk_direction_order);

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
         for (auto yChunk = world_dimensions.y - (yStage + 1); yChunk >= 0; yChunk -= 2)
         {
            if (yChunk < 0)
               continue;

            ++thread_pool_tasks;

            // Start
            const auto chunkIndex = IVec2(xChunk, yChunk);

            // Submit a lambda object to the pool.
            post(thread_pool, [this, chunkIndex]() mutable
               {
                  //Now we know our chunk indexes we create a local group to simplify lookup
                  auto* localPixels = chunks[chunkIndex]->pixel_data;

                  auto* neighbourPixels = localPixels;

                  bool* isProcessed = is_chunk_processed[chunkIndex];
                  bool* neighbourIsProcessed = isProcessed;

                  E_PixelType returnPixels[2];

                  WorldChunk** neighbourChunks = chunks[chunkIndex]->neighbour_chunks;

                  //TODO Work out the best way to organize this
                  int pieceOrder[] =
                  {
                     (x_dir_ == 0 ? 2 : 1),
                     3,
                     4,
                     0,
                     (x_dir_ == 0 ? 1 : 2),
                  };

                  bool isPixelsLocal = true;
                  for (auto piece : pieceOrder)
                  {
                     if (piece == 4)
                     {
                        neighbourPixels = localPixels;
                        neighbourIsProcessed = isProcessed;
                        isPixelsLocal = true;
                     }
                     /// <summary>
                     /// Our Inner Chunk Update
                     /// </summary>
                     for (auto
                        x = x_loop_from_to_dir_[piece][x_dir_][From];
                        x != x_loop_from_to_dir_[piece][x_dir_][To];
                        x += x_loop_from_to_dir_[piece][x_dir_][Dir])
                     {
                        for (auto
                           y = y_loop_from_to_dir_[piece][y_dir_][From];
                           y != y_loop_from_to_dir_[piece][y_dir_][To];
                           y += y_loop_from_to_dir_[piece][y_dir_][Dir])
                        {
                           const short localIndex = (y * Constant::chunk_size_x) + x;

                           // //? DEBUG
                           // localPixels[localIndex] = 0x00000000;
                           // switch (i)
                           // {
                           // // case 0:
                           // //    localPixels[localIndex] += 0x40000000;
                           // //    break;
                           // // case 1:
                           // //    localPixels[localIndex] += 0x00400000;
                           // //    break;
                           // case 2: 
                           //   localPixels[localIndex] += 0x00004000; // These ones
                           //   break;
                           // case 3:
                           //    localPixels[localIndex] += 0x40000000; // These ones
                           //    break;
                           // }
                           // continue;
                           // //? DEBUG

                           // If the pixel is empty, or something beat us to update it
                           if (localPixels[localIndex] == 0 || isProcessed[localIndex]) continue;

                           BasePixel* pixel = world_data_handler->GetPixelFromIndex(Utility::GetType(localPixels[localIndex]));

                           const short* pixelDirOrder = chunk_direction_order[pixel->pixel_index];
                           for (auto directionIndex = 0; directionIndex < static_cast<short>(DIR_COUNT); directionIndex++)
                           {
                              short direction = pixelDirOrder[directionIndex];
                              // If Direction is DIR_COUNT all other values will be DIR_COUNT and can be safely aborted.
                              if (direction == DIR_COUNT) break;

                              // This will be the case for most updates.
                              if (!isPixelsLocal)
                              {
                                 neighbourPixels = localPixels;
                                 neighbourIsProcessed = isProcessed;
                                 isPixelsLocal = true;
                              }

                              short neighbourIndex;
                              uint32_t pixelIndexChange = 0;

                              uint8_t maxPixelRange = pixel->MaxUpdateRange();
                              uint8_t borderRange = GetDistanceToBorder(x, y, direction);

                              switch (piece)
                              {
                              case 4: // Inner Chunk (No Chunk traversal)
                                 neighbourIndex = localIndex + Constant::pixel_index_direction_change[direction];
                                 break;
                              case 0: // North
                                 if ((localIndex == Constant::chunk_n_w_corner_index || localIndex == Constant::chunk_n_e_corner_index) && !(direction == E_ChunkDirection::South || direction == E_ChunkDirection::North))
                                    continue;
                                 pixelIndexChange = Constant::pixel_index_north_border[direction];
                                 direction = E_ChunkDirection::North;
                                 break;
                              case 1: // East
                                 pixelIndexChange = Constant::pixel_index_east_border[direction];
                                 direction = E_ChunkDirection::East;
                                 break;
                              case 2: // West
                                 pixelIndexChange = Constant::pixel_index_west_border[direction];
                                 direction = E_ChunkDirection::West;
                                 break;
                              case 3: // South
                                 if ((localIndex == Constant::chunk_s_w_corner_index || localIndex == Constant::chunk_s_e_corner_index) && !(direction == E_ChunkDirection::South || direction == E_ChunkDirection::North))
                                    continue;
                                 pixelIndexChange = Constant::pixel_index_south_border[direction];
                                 direction = E_ChunkDirection::South;
                                 break;
                              }

                              // Piece 4 is Internal Chunk, we know we won't need to worry about chunk traversal
                              if (piece != 4) {
                                 if ((pixelIndexChange) & (1U << Constant::new_chunk_bit))
                                 {
                                    if (neighbourChunks[direction] == nullptr)
                                       continue;
                                    //? change &= ~(1 << Constant::new_chunk_bit);
                                    //TODO We could probably just store what our chunk is and do this if we have to do any updates.
                                    neighbourPixels = neighbourChunks[direction]->pixel_data;
                                    neighbourIsProcessed = is_chunk_processed[neighbourChunks[direction]->position];
                                    isPixelsLocal = false;
                                 }
                                 //? change &= ~(1 << Constant::negative_bit);
                                 if (pixelIndexChange & (1U << Constant::negative_bit))
                                    neighbourIndex = localIndex - static_cast<short>(pixelIndexChange);
                                 else
                                    neighbourIndex = localIndex + static_cast<short>(pixelIndexChange);
                              }

                              auto* pixelNeighbour = world_data_handler->GetPixelFromIndex(Utility::GetType(neighbourPixels[neighbourIndex]));
#ifdef DEBUG_GAME
                              if (pixelNeighbour == nullptr) {
                                 printf("WARNING: pixelNeighbour returned NULL\n");
                                 continue;
                              }
#endif
                              const auto neighbourType = pixelNeighbour->GetType();

                              // Now we ask the Pixel what it wants to do with its neighbour
                              int8_t result = CheckLogic(pixelDirOrder[directionIndex], pixel, neighbourType,
                                 returnPixels);

                              if (DEBUG_PrintPixelData)
                              {
                                 printf("P-%i\t%i->%i\tX: %i\tY:%i\tUPDATE:%i\t%i\t->\t%i\n", piece,
                                    pixelDirOrder[directionIndex], direction, x, y, DEBUG_FrameCounter, localIndex,
                                    neighbourIndex);
                              }

                              switch (result)
                              {
                              case E_LogicResults::SuccessUpdate:
                                 std::swap(localPixels[localIndex], neighbourPixels[neighbourIndex]);
                                 neighbourIsProcessed[neighbourIndex] = true;
                                 break;


                              case E_LogicResults::FirstReturnPixel:
                                 localPixels[localIndex] = world_data_handler->GetNewPixelOfType(returnPixels[0]);
                                 isProcessed[localIndex] = true;
                                 break;
                              case E_LogicResults::SecondReturnPixel:
                                 neighbourPixels[neighbourIndex] = world_data_handler->GetNewPixelOfType(returnPixels[1]);
                                 neighbourIsProcessed[neighbourIndex] = true;
                                 break;
                              case E_LogicResults::DualReturnPixel:
                                 localPixels[localIndex] = world_data_handler->GetNewPixelOfType(returnPixels[0]);
                                 isProcessed[localIndex] = true;

                                 neighbourPixels[neighbourIndex] = world_data_handler->GetNewPixelOfType(returnPixels[1]);
                                 neighbourIsProcessed[neighbourIndex] = true;
                                 break;
                              case E_LogicResults::NoChange:
                                 break;

                              case E_LogicResults::FailedUpdate:
                              case E_LogicResults::None:
                              default:
                                 continue;
                              }

                              break;
                           }
                        }

                     }
                  }
                  --thread_pool_tasks;
               });
         }
      }
      while (thread_pool_tasks > 0)
      {
         // Sleep(1);
      }
   }

   //? while (!used_processed_queue.empty())
   //? {
   //?    bool* data;
   //?    used_processed_queue.pop(data);
   memset(is_processed_array_, false, Constant::chunk_total_size * world_dimensions.x * world_dimensions.y);
   //?    is_processed_queue.push(data);
   //? }
   //TODO Improve this?
   for (auto y = 0; y < world_dimensions.y; y++)
   {
      for (auto x = 0; x < world_dimensions.x; x++)
      {
         is_chunk_processed[IVec2(x, y)] = nullptr;
      }
   }
}

inline uint8_t WorldSimulator::GetDistanceToBorder(const short x, const short y, const short direction)
{
   switch (direction)
   {
   case North:
      return y + 1;
   case NorthEast:
      return std::min(y + 1, Constant::chunk_size_x - x);
   case East:
      return Constant::chunk_size_x - x;
   case SouthEast:
      return std::min(Constant::chunk_size_x - x, Constant::chunk_size_y - y);
   case South:
      return Constant::chunk_size_y - y;
   case SouthWest:
      return std::min(Constant::chunk_size_y - y, x + 1);
   case West:
      return x + 1;
   case NorthWest:
      return std::min(x + 1, y + 1);
   }
}

inline bool WorldSimulator::DoesChunkHaveNeighbour(WorldChunk** neighbours, const short direction)
{
   return neighbours[direction] != nullptr;
}

inline int8_t WorldSimulator::CheckLogic(const int direction, BasePixel* pixel, const E_PixelType neighbour_type,
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
      return E_LogicResults::FailedUpdate;
   }
}


void WorldSimulator::UpdateInput()
{
   InputManager* input = InputManager::Instance();
   const IVec2 mousePos = input->MousePosition();
   const IVec2 lastFramePos = input->LastMousePosition();


   if (input->GetMouseButton(MouseLeft))
   {
      //TODO Improve
      float distance = IVec2::Distance(mousePos, lastFramePos);
      int steps = (distance / DEBUG_PenSize) + 1;
      for (int i = 0; i <= steps; i++)
      {
         IVec2 pixel = IVec2::Lerp(lastFramePos, mousePos, (1.0f / steps) * i);
         Pen(pixel, paint_manager->selected_pixel, DEBUG_PenSize);
      }
   }
   if (input->GetMouseButton(MouseMiddle))
   {
      Pen(mousePos, paint_manager->selected_pixel, DEBUG_PenSize, false);
   }
   if (input->GetMouseButton(MouseRight))
   {
      Pen(mousePos, world_data_handler->GetPixelFromIndex(0), DEBUG_PenSize);
   }
   if (input->GetMouseButton(MouseMiddle))
   {
      Pen(mousePos, paint_manager->selected_pixel, DEBUG_PenSize, false);
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


   //TODO Save/Load Manager?
   if (input->GetKeyDown((KeyCode::SemiColon)))
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
         memset(chunks[IVec2(x, y)]->pixel_data, 0, Constant::chunk_total_size * sizeof(Uint32));
      }
   }
}

bool WorldSimulator::Draw(Camera* camera)
{
   glm::vec3 camPos = camera->GetPosition();
   const IVec2 screenSize = game_settings->screen_size;

   int xChunkStart = static_cast<int>(floor(camPos.x / Constant::chunk_size_x));
   int xChunkEnd = xChunkStart + (screenSize.x / Constant::chunk_size_x) + 2;
   int xOffset = static_cast<int>(camPos.x) % Constant::chunk_size_x;
   if (camPos.x < 0)
      xChunkStart += 1;

   int yChunkStart = static_cast<int>(floor(camPos.y / Constant::chunk_size_y));
   int yChunkEnd = yChunkStart + (screenSize.y / Constant::chunk_size_y) + 2;
   int yOffset = static_cast<int>(camPos.y) % Constant::chunk_size_y;
   if (camPos.y < 0)
      yChunkStart += 1;

   for (int xVal = xChunkStart; xVal < xChunkEnd; xVal++)
   {
      for (int yVal = yChunkStart; yVal < yChunkEnd; yVal++)
      {
         auto worldChunk = chunks.find(IVec2(xVal, yVal));
         // Chunk doesn't exist, we don't render
         if (worldChunk == chunks.end())
            continue;

         glUseProgram(game_settings->default_shader);
         
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, map_textures);

         glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Constant::chunk_size_x, Constant::chunk_size_y, GL_RED_INTEGER, GL_UNSIGNED_INT, worldChunk->second->pixel_data);

         glm::mat4 model = glm::mat4(1.0f);
         glm::vec3 modelPosition = glm::vec3(
            -xOffset + (((xVal - xChunkStart) + 1) * Constant::chunk_size_x) - (Constant::chunk_size_x / 2) + xVal,
            -yOffset + (((yVal - yChunkStart) + 1) * Constant::chunk_size_y) - (Constant::chunk_size_y / 2) + yVal,
            1.0f
         );
         model = glm::translate(model, modelPosition);

         model = glm::scale(model, glm::vec3(128, 128, 1.0f));

         int modelLoc = glGetUniformLocation(game_settings->default_shader, "model");
         glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
         int projLoc = glGetUniformLocation(game_settings->default_shader, "projection");
         glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->GetProjection()));


         glBindVertexArray(VAO);
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      }
   }

   //? //TODO Replace this with a better zoom
   //? SDL_Rect zoomrect = world_render_rect;
   //? zoomrect.w *= DEBUG_ZoomLevel;
   //? zoomrect.h *= DEBUG_ZoomLevel;

   return true;
}

void WorldSimulator::DebugShowChunkProcessPieces()
{
   Uint32 pieceOrderTestColour[] = {
      0xFF0000FF, // Red
      0x0000FFFF, // Blue
      0xFFFF00FF, // Yellow
      0x00FF00FF, // Green
      0xFFFFFFFF, // White
      0xCC00CCCC // Purple
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
                  auto* localPixels = chunks[chunkIndex]->pixel_data;
                  const short localIndex = (y * Constant::chunk_size_x) + x;
                  localPixels[localIndex] = pieceOrderTestColour[piece];
               }
            }
         }
      }
   }
}

void WorldSimulator::DebugDrawPixelRange()
{
   if (!InputManager::Instance()->GetKeyDown(KeyCode::R))
      return;

   ClearWorld();

   Uint32 pixelDepthColour[] = {
   0x0000FFFF, // Blue
   0x00FF00FF, // Green
   0x00e500FF, // Green
   0x00cc00FF, // Green
   0x00b200FF, // Green
   0x009900FF, // Green
   0x007f00FF, // Green
   0x006600FF, // Green
   0x004c00FF, // Green
   0x003300FF, // Green
   };

   IVec2 virtualMouse = game_settings->virtual_mouse;
   if (virtualMouse.x < 0 || virtualMouse.y < 0)
      return;
   if (virtualMouse.x > world_dimensions.x * Constant::chunk_size_x || virtualMouse.y > world_dimensions.y * Constant::chunk_size_y)
      return;

   const auto chunkIndex = IVec2(virtualMouse.x / Constant::chunk_size_x, virtualMouse.y / Constant::chunk_size_y);
   auto* localPixels = chunks[chunkIndex]->pixel_data;

   const auto pixelPos = IVec2(virtualMouse.x % Constant::chunk_size_x, virtualMouse.y % Constant::chunk_size_y);

   const short pixelIndex = (pixelPos.y * Constant::chunk_size_x) + pixelPos.x;

   world_data_handler->FillWithPixelUpdateOrders(chunk_direction_order);

   BasePixel* pixel = paint_manager->selected_pixel;
   short maxPixelRange = paint_manager->selected_pixel->MaxUpdateRange();

   const short* pixelDirOrder = chunk_direction_order[pixel->pixel_index];
   for (auto directionIndex = 0; directionIndex < static_cast<short>(DIR_COUNT); directionIndex++)
   {
      int direction = pixelDirOrder[directionIndex];
      // If Direction is DIR_COUNT all other values will be DIR_COUNT and can be safely aborted.
      if (direction == DIR_COUNT) break;

      short borderRange = GetDistanceToBorder(pixelPos.x, pixelPos.y, direction);
      int neighbourIndex = pixelIndex;
      for (int pixelRange = 1; pixelRange <= maxPixelRange; pixelRange++)
      {
         if (pixelRange >= borderRange)
            break;

         neighbourIndex = neighbourIndex + Constant::pixel_index_direction_change[direction];
         localPixels[neighbourIndex] = pixelDepthColour[pixelRange];
      }
   }
   localPixels[pixelIndex] = pixelDepthColour[0];
}
