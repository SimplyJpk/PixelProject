#include "WorldSimulator.h"
#include <math.h>
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
		// Allocate our pixels and processing bools
		ChunkTotalSize = CHUNK_SIZE_X * CHUNK_SIZE_Y;

		//gameSettings->Screen_Size = IVec2(WORLD_DIMENSIONS.x;
		// pixels = new Uint32[WORLD_DIMENSIONS.x, WORLD_DIMENSIONS.y, ChunkTotalSize];
		// isProcessed = new bool[WORLD_DIMENSIONS.x, WORLD_DIMENSIONS.y, ChunkTotalSize];

		// Generate our chunks and the pixel data
		for (int y = 0; y < WORLD_DIMENSIONS.y; y++)
		{
				for (int x = 0; x < WORLD_DIMENSIONS.x; x++)
				{
						// Create and store our chunk
						WorldChunk* newChunk = new WorldChunk(IVec2(x, y));
						// Add chunk to map
						chunks.insert(std::make_pair(IVec2(x, y), newChunk));
						//x worldChunks.push_back(newChunk);
						// Allocate its pixels/data
						newChunk->pixels = new Uint32[ChunkTotalSize] { 0 };


				}
		}
		// Loop through once more, but this time we build a list of neighbours for each chunk for quicker access
		for (int y = 0; y < WORLD_DIMENSIONS.y; y++)
		{
				for (int x = 0; x < WORLD_DIMENSIONS.x; x++)
				{
						int chunkIndex = (y * WORLD_DIMENSIONS.x) + x;
						WorldChunk* localChunk = chunks[IVec2(x, y)];

						// If North is free
						if (y > 0) {
								localChunk->SetNeighbour(North, chunks[IVec2(x, y - 1)]);
								// North West
								if (x > 0)
										localChunk->SetNeighbour(NorthWest, chunks[IVec2(x - 1, y - 1)]);
								// North East
								if (x < WORLD_DIMENSIONS.x - 1)
										localChunk->SetNeighbour(NorthEast, chunks[IVec2(x + 1, y - 1)]);
						}
						// South
						if (y < WORLD_DIMENSIONS.y - 1) {
								localChunk->SetNeighbour(South, chunks[IVec2(x, y + 1)]);
								// South West
								if (x > 0)
										localChunk->SetNeighbour(SouthWest, chunks[IVec2(x - 1, y + 1)]);
								// South East
								if (x < WORLD_DIMENSIONS.x - 1)
										localChunk->SetNeighbour(SouthEast, chunks[IVec2(x + 1, y + 1)]);
						}
						// East West
						if (x > 0) 
								localChunk->SetNeighbour(West, chunks[IVec2(x - 1, y)]);
						if (x < WORLD_DIMENSIONS.x - 1)
								localChunk->SetNeighbour(ChunkDirection::East, chunks[IVec2(x + 1, y)]);
				}
		}

		// Create our world texture, we use this to render the world chunks.
		worldTexture = SDL_CreateTexture(gameRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, gameSettings->Screen_Size.x + (CHUNK_SIZE_X * 2), gameSettings->Screen_Size.y + (CHUNK_SIZE_Y * 2));
		if (worldTexture == NULL) {
				printf("WorldTexture failed to Init\nError:%s\n", SDL_GetError());
				SDL_ClearError();
		}
}

void WorldSimulator::Pen(IVec2 point, BasePixel* pixelType, int size)
{
		// Grab our CameraPosition in Pixels
		SDL_Rect cameraPos = cam->viewPort;
		// Calculate the Position of the Camera in the world in Chunks
		Vec2 cameraWorldPosition = Vec2((float)cameraPos.x / CHUNK_SIZE_X, (float)cameraPos.y / CHUNK_SIZE_Y);
		// Same as above, but now we round to floor
		IVec2 cameraChunk = IVec2(cameraWorldPosition.x, cameraWorldPosition.y);
		// Now we remove our position to get our offset
		IVec2 cameraWorldOffset = IVec2((cameraWorldPosition.x - cameraChunk.x) * CHUNK_SIZE_X, (cameraWorldPosition.y - cameraChunk.y) * CHUNK_SIZE_Y);

		short min = 0, max = pixelType->ColourCount;
		//TODO Fix this? Error based on drawing, likely on Line 111 when we're just passing our CameraChunk in without any additional checks

		//TODO A lot of math, we should be able to simplify this easily enough by storing within scope. 
		//TODO IE: (WORLD_DIM * CHUNK_DIM) This'll likely never change?
		if (point.x >= 0 && point.x < WORLD_DIMENSIONS.x * CHUNK_SIZE_X) {
				if (point.y >= 0 && point.y <= WORLD_DIMENSIONS.y * CHUNK_SIZE_Y) {
						for (int x = point.x - size; x < point.x + size; x++)
						{
								// We skip calculations if our position is outside the realm of our world.
								if (x < 0 || x > WORLD_DIMENSIONS.x * CHUNK_SIZE_X - 1) continue;
								// We get the floor of X divided by Size of Chunks
								static int xFloor;
								xFloor = floor(x / CHUNK_SIZE_X);
								if (xFloor > WORLD_DIMENSIONS.x) continue;
								for (int y = point.y - size; y < point.y + size; y++)
								{
										if (y < 0 || y > WORLD_DIMENSIONS.y * CHUNK_SIZE_Y - 1) continue;
										static int yFloor;
										yFloor = floor(y / CHUNK_SIZE_Y);
										if (yFloor > WORLD_DIMENSIONS.y) continue;

										chunks[IVec2(xFloor + cameraChunk.x, yFloor + cameraChunk.y)]->pixels[
												((y - (yFloor * CHUNK_SIZE_Y) - cameraWorldOffset.y) * CHUNK_SIZE_X) + (x - (xFloor * CHUNK_SIZE_X) - cameraWorldOffset.x)
										] = pixelType->TypeColours[(rng() % max)];

												if (size == 1) return;
								}
						}
				}
		}
}

void WorldSimulator::Update() {
		//TODO Remvoe this
		DEBUG_FrameCounter++;
		if (DEBUG_DropSand) {
				BasePixel* sandPixel = worldDataHandler->GetPixelFromIndex(3);
				short maxSandColours = sandPixel->ColourCount;
				for (int x = 0; x < WORLD_DIMENSIONS.x; x++)
				{
						for (int xDim = 0; xDim < CHUNK_SIZE_X; xDim++)
						{
								if (rng() % DEBUG_SandDropRate == 0) {
										chunks[IVec2(x, 0)]->pixels[(xDim * 0) + xDim] = sandPixel->TypeColours[0, (rng() % maxSandColours)];
								}
						}
				}
		}
		//TODO Remove above

		int xLimits = WORLD_DIMENSIONS.x / 2;
		int yLimits = WORLD_DIMENSIONS.y / 2;
		int worldLimits = xLimits * yLimits;

		int xStage, yStage;
		int chunkUpdates = 0;

		//? // TODO Clean this up somehow, we need to have some way to add some additional noise to pixel movement?
		//? // if (rng() % 150 == 0)
		//? // {
		//? // 		xDir = (rng() % 2 == 0 ? 1 : -1);
		//? // 		yDir = (rng() % 2 == 0 ? 1 : -1);
		//? // }
		//? // ^

		xDir = (xDir == -1 ? 1 : -1);
		yDir = (yDir == -1 ? 1 : -1);

		for (int i = 0; i < 4; i++)
		{
				switch (i)
				{
				case 0: xStage = 0; yStage = 0; break;
				case 1: xStage = 1; yStage = 1; break;
				case 2: xStage = 1; yStage = 0; break;
				case 3: xStage = 0; yStage = 1; break;
				}

				int pendingTasks = 0;

				for (int xChunk = xStage; xChunk < WORLD_DIMENSIONS.x; xChunk += 2)
				{
						if (xChunk >= WORLD_DIMENSIONS.x)
								continue;
						for (int yChunk = yStage; yChunk < WORLD_DIMENSIONS.y; yChunk += 2)
						{
								if (yChunk >= WORLD_DIMENSIONS.y)
										continue;

								// std::async(UpdateChunk(IVec2(xChunk, yChunk), isProcessedData));
								threadPoolTasks++;
								// Submit a lambda object to the pool.
								boost::asio::post(threadPool, [this, xChunk, yChunk, chunkUpdates]()
										mutable {
												// UpdateChunk(xChunk, yChunk, chunkUpdates);
												// Start
												IVec2 chunkIndex = IVec2(xChunk, yChunk);
												//? May need this later?
												int _localChunkIndex = (yChunk * WORLD_DIMENSIONS.x) + xChunk;
												//TODO We should pool these so when I chunk this properly we can use only a handful of arrays
												//Now we know our chunk indexes we create a local group to simplify lookup
												Uint32* localPixels = chunks[chunkIndex]->pixels; // [_localChunkIndex] ->pixels;
												bool* isProcessed = isProcessedQueue[chunkUpdates];

												BasePixel* neighbour;
												int neighbourIndex;
												E_PixelType neighbourType;
												E_PixelType returnPixels[2]{ E_PixelType::Space };

												short** ChunkDirectionOrder = ChunkDirectionOrderContainers[chunkUpdates];
												// ChunkDirectionOrderconst short* ChunkDirectionOrders[static_cast<short>(E_PixelType::COUNT)];
												worldDataHandler->FillWithPixelUpdateOrders(ChunkDirectionOrder);

												// Counter
												short xStart = 1;
												short yStart = 1;
												// End
												short xTo = CHUNK_SIZE_X - 2;
												short yTo = CHUNK_SIZE_Y - 2;

												// Initialize
												if (xDir == -1) {
														xStart = CHUNK_SIZE_X - 2;
														xTo = 1;
												}
												if (yDir == -1) {
														yStart = CHUNK_SIZE_Y - 2;
														yTo = 1;
												}


												for (short y = yStart; y != yTo; y+= yDir)
												{
														for (short x = xStart; x != xTo; x += xDir) {
																const short _localIndex = (y * CHUNK_SIZE_X) + x;

																if (!(localPixels[_localIndex] != 0 && !isProcessed[_localIndex])) continue;

																BasePixel* pixel = worldDataHandler->GetPixelFromPixelColour(localPixels[_localIndex]);
																E_PixelType pixelType = pixel->GetType();

																//? // TODO Can we make some sort of conditional array?
																//? neighbourQualifiers =
																//? {
																//? 		(y > 0), // N
																//? 		(y > 0) && (x < CHUNK_SIZE_X - 1), // NE
																//? 		(x < CHUNK_SIZE_X - 1), // E
																//? 		(y < CHUNK_SIZE_Y - 1) && (x < CHUNK_SIZE_X - 1), // SE
																//? 		(y < CHUNK_SIZE_Y - 1), // S
																//? 		(y < CHUNK_SIZE_Y - 1) && (x > 0), // SW
																//? 		(x > 0), // W
																//? 		(y > 0) && (x > 0), // NW
																//? };

																//TODO This could be slow? Can we pull one of these each frame instead of every time a pixel updates?
																// auto ChunkDirectionOrder = pixel->GetSingleChunkOrder();

																const short* PixelDirOrder = ChunkDirectionOrder[pixel->PixelIndex];
																for (int i = 0; i < static_cast<short>(ChunkDirection::DIR_COUNT); i++) {
																		const int DirIndex = PixelDirOrder[i];
																		if (DirIndex == ChunkDirection::DIR_COUNT) continue;

																		switch (DirIndex)
																		{
																		case ChunkDirection::North:
																				neighbourIndex = _localIndex - CHUNK_SIZE_X;
																				break;
																		case ChunkDirection::NorthEast:
																				neighbourIndex = _localIndex - CHUNK_SIZE_X + 1;
																				break;
																		case ChunkDirection::East:
																				neighbourIndex = _localIndex + 1;
																				break;
																		case ChunkDirection::SouthEast:
																				neighbourIndex = _localIndex + CHUNK_SIZE_X + 1;
																				break;
																		case ChunkDirection::South:
																				neighbourIndex = _localIndex + CHUNK_SIZE_X;
																				break;
																		case ChunkDirection::SouthWest:
																				neighbourIndex = _localIndex + CHUNK_SIZE_X - 1;
																				break;
																		case ChunkDirection::West:
																				neighbourIndex = _localIndex - 1;
																				break;
																		case ChunkDirection::NorthWest:
																				neighbourIndex = _localIndex - CHUNK_SIZE_X - 1;
																				break;
																		}

																		neighbour = worldDataHandler->GetPixelFromPixelColour(localPixels[neighbourIndex]);
																		neighbourType = neighbour->GetType();

																		// Now we ask the Pixel what it wants to do with its neighbour
																		// this can return true/false and if the pixel needs to convert the neighbour to something returnPixels will have different values
																		bool result;
																		switch (DirIndex)
																		{
																		case ChunkDirection::North:
																				result = pixel->N_Logic(neighbourType, returnPixels);
																				break;
																		case ChunkDirection::NorthEast:
																				result = pixel->NE_Logic(neighbourType, returnPixels);
																				break;
																		case ChunkDirection::East:
																				result = pixel->E_Logic(neighbourType, returnPixels);
																				break;
																		case ChunkDirection::SouthEast:
																				result = pixel->SE_Logic(neighbourType, returnPixels);
																				break;
																		case ChunkDirection::South:
																				result = pixel->S_Logic(neighbourType, returnPixels);
																				break;
																		case ChunkDirection::SouthWest:
																				result = pixel->SW_Logic(neighbourType, returnPixels);
																				break;
																		case ChunkDirection::West:
																				result = pixel->W_Logic(neighbourType, returnPixels);
																				break;
																		case ChunkDirection::NorthWest:
																				result = pixel->NW_Logic(neighbourType, returnPixels);
																				break;
																		}
																		if (result) {
																				if (DEBUG_PrintPixelData) { printf("X: %i\tY:%i\tUPDATE:%i\t(IN)\n", x, y, DEBUG_FrameCounter); }
																				if (returnPixels[0] != returnPixels[1]) {
																						if (pixelType != returnPixels[0]) {
																								chunks[chunkIndex]->pixels[_localIndex] = worldDataHandler->GetPixelFromType(returnPixels[0])->GetRandomColour();
																						}
																						if (neighbourType != returnPixels[1]) {
																								chunks[chunkIndex]->pixels[_localIndex] = worldDataHandler->GetPixelFromType(returnPixels[1])->GetRandomColour();
																						}
																				}
																				else {
																						std::swap(chunks[chunkIndex]->pixels[_localIndex], chunks[chunkIndex]->pixels[neighbourIndex]);
																				}
																				isProcessed[neighbourIndex] = true;
																				break;
																		}
																}

																//TODO Replace this
																//! // Check if we're not on the bottom
																//! if (y < CHUNK_SIZE_Y - 1) {
																//! 		// Down
																//!  		_adjustedIndex = ((y + 1) * CHUNK_SIZE_X) + x;
																//! 		if (isProcessed[_adjustedIndex]) continue;
																//! 
																//! 		if (isProcessed[_adjustedIndex] = ChunkUtility::MovePixelSameChunk(localPixels, _localIndex, _adjustedIndex)) continue;
																//! 		else {
																//! 				// Down Diagonal
																//! 				if (LoopDir.x == 1) {
																//! 						// Same Chunk
																//! 						if (x < (CHUNK_SIZE_X - 2)) {
																//! 								if (isProcessed[_adjustedIndex] = ChunkUtility::MovePixelSameChunk(localPixels, _localIndex, _adjustedIndex + 1)) continue;
																//! 						}
																//! 						else if (chunkIndex.x < WORLD_DIMENSIONS.x - 1) {
																//! 								if (isProcessed[_adjustedIndex] = 
																//! 										ChunkUtility::MovePixel(localPixels, _localIndex, ((y + 2) * CHUNK_SIZE_X) , chunks[chunkIndex]->neighbourChunks//[ChunkDirection::East]->p ixels)) ?o!ntinue;
																//! 						}
																//! 				}
																//! 				else if (LoopDir.x == -1) {
																//! 						// Same Chunk
																//! 						if (x > 0) {
																//! 								if (isProcessed[_adjustedIndex] = ChunkUtility::MovePixelSameChunk(localPixels, _localIndex, _adjustedIndex - 1)) continue;
																//! 						}
																//! 						else if (chunkIndex.x > 0) {
																//! 								if (isProcessed[_adjustedIndex] =
																//! 										ChunkUtility::MovePixel(localPixels, _localIndex, ((y + 2) * CHUNK_SIZE_X) - 1, chunks[chunkIndex]->neighbourChunks//[ChunkDirection::West]->?pi x! s)) continue;
																//! 						}
																//! 				}
																//! 		}
																//! }
																//! else {
																//! 		// Down into new Chunk
																//! 		if (chunkIndex.y < WORLD_DIMENSIONS.y - 1) {
																//! 				ChunkUtility::MovePixel(chunks[chunkIndex]->pixels, _localIndex, x, chunks[chunkIndex]->neighbourChunks[ChunkDirection::South]->pixels);
																//! 		}
																//! }
														}
												}

												//? We could hold onto our 8 neighbouring chunks pixels here, and almost brute force this with switch statements? WOuld be super ugly..
												//? Not sure how else to do all this..
											
												WorldChunk** neighbourChunks = chunks[chunkIndex]->neighbourChunks;
												for (short ShellUpdate = 0; ShellUpdate < 4; ShellUpdate++)
												{
														GetStartAndToForLoop(ShellUpdate, xStart, xTo, yStart, yTo);
														for (short y = yStart; y != yTo; y += yDir)
														{
																for (short x = xStart; x != xTo; x += xDir) {
																		const short _localIndex = (y * CHUNK_SIZE_X) + x;
																		if (!(localPixels[_localIndex] != 0 && !isProcessed[_localIndex])) continue;

																		BasePixel* pixel = worldDataHandler->GetPixelFromPixelColour(localPixels[_localIndex]);
																		E_PixelType pixelType = pixel->GetType();

																		const short* PixelDirOrder = ChunkDirectionOrder[pixel->PixelIndex];
																		for (int i = 0; i < static_cast<short>(ChunkDirection::DIR_COUNT); i++) {
																				int DirIndex = PixelDirOrder[i];
																				if (DirIndex == ChunkDirection::DIR_COUNT) continue;

																				bool isSameChunk = true;

																				neighbour = nullptr;
																				switch (DirIndex)
																				{
																				// case ChunkDirection::NorthEast:
																				// 		if (x == CHUNK_SIZE_X - 1) {
																				// 				if (neighbourChunks[ChunkDirection::East] != nullptr) {
																				// 						neighbourIndex = (CHUNK_SIZE_X * (y)) - 1;
																				// 						neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[ChunkDirection::East]->pixels[neighbourIndex]);
																				// 						newChunkDir = ChunkDirection::East;
																				// 						isSameChunk = false;
																				// 						break;
																				// 				}
																				// 		}
																				// 		continue;
																				// case ChunkDirection::NorthWest:
																				// 		if (x == 0) {
																				// 				if (neighbourChunks[ChunkDirection::West] != nullptr) {
																				// 						neighbourIndex = (CHUNK_SIZE_X * (y)) - 1;
																				// 						neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[ChunkDirection::West]->pixels[neighbourIndex]);
																				// 						newChunkDir = ChunkDirection::West;
																				// 						isSameChunk = false;
																				// 						break;
																				// 				}
																				// 		}
																				// 		continue;
																				// case ChunkDirection::North:
																				// 		if (y == 0) {
																				// 				if (neighbourChunks[ChunkDirection::North] == nullptr) continue;
																				// 				neighbourIndex = ChunkTotalSize - x;
																				// 				neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[ChunkDirection::North]->pixels[neighbourIndex]);
																				// 				newChunkDir = ChunkDirection::North;
																				// 				isSameChunk = false;
																				// 		}
																				// 		else {
																				// 				neighbourIndex = _localIndex - CHUNK_SIZE_X;
																				// 		}
																				// 		break;
																				// case ChunkDirection::SouthEast:
																				// 		if (x == CHUNK_SIZE_X - 1) {
																				// 				if (neighbourChunks[ChunkDirection::East] != nullptr) {
																				// 						neighbourIndex = (CHUNK_SIZE_X * (y)) + 1;
																				// 						neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[ChunkDirection::East]->pixels[neighbourIndex]);
																				// 						newChunkDir = ChunkDirection::East;
																				// 						isSameChunk = false;
																				// 						break;
																				// 				}
																				// 		}
																				// case ChunkDirection::SouthWest:
																				// 		if (x == 0) {
																				// 				if (neighbourChunks[ChunkDirection::West] != nullptr) {
																				// 						neighbourIndex = (CHUNK_SIZE_X * (y)) - 1;
																				// 						neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[ChunkDirection::West]->pixels[neighbourIndex]);
																				// 						newChunkDir = ChunkDirection::West;
																				// 						isSameChunk = false;
																				// 						break;
																				// 				}
																				// 		}
																				case ChunkDirection::South: {
																						if (y == CHUNK_SIZE_Y - 1) {
																								if (neighbourChunks[ChunkDirection::South] == nullptr) { continue; }
																								neighbourIndex = x;
																								DirIndex = ChunkDirection::South;
																								neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[DirIndex]->pixels[neighbourIndex]);
																						}
																						else { neighbourIndex = _localIndex + CHUNK_SIZE_X; }
																						break;
																				}
																				case ChunkDirection::West: {
																						if (x == 0) {
																								if (neighbourChunks[ChunkDirection::West] == nullptr) { continue; }
																								neighbourIndex = (CHUNK_SIZE_X * (y + 1)) - 1;
																								DirIndex = ChunkDirection::West;
																								neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[DirIndex]->pixels[neighbourIndex]);
																						}
																						else { neighbourIndex = _localIndex - 1; }
																						break;
																				}
																				case ChunkDirection::East: {
																						if (x == CHUNK_SIZE_X - 1) {
																								if (neighbourChunks[ChunkDirection::East] == nullptr) { continue; }
																								neighbourIndex = (CHUNK_SIZE_X * (y)) + 1;
																								DirIndex = ChunkDirection::East;
																								neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[DirIndex]->pixels[neighbourIndex]);
																						}
																						else { neighbourIndex = _localIndex + 1; }
																						break;
																				}
																				// case ChunkDirection::East:
																				// 		if (x == CHUNK_SIZE_X - 1) {
																				// 				if (neighbourChunks[ChunkDirection::East] != nullptr) {
																				// 						neighbourIndex = (CHUNK_SIZE_X * (y)) - 1;
																				// 						neighbour = worldDataHandler->GetPixelFromPixelColour(neighbourChunks[ChunkDirection::East]->pixels[neighbourIndex]);
																				// 						newChunkDir = ChunkDirection::East;
																				// 						isSameChunk = false;
																				// 						break;
																				// 				}
																				// 		}
																				// 		continue;
																				default: 
																						continue;
																				}


																				if (neighbour == nullptr)
																						neighbour = worldDataHandler->GetPixelFromPixelColour(localPixels[neighbourIndex]);
																				else
																						isSameChunk = false;

																				neighbourType = neighbour->GetType();

																				// Now we ask the Pixel what it wants to do with its neighbour
																				// this can return true/false and if the pixel needs to convert the neighbour to something returnPixels will have different values
																				bool result;
																				switch (DirIndex)
																				{
																				case ChunkDirection::North:
																						result = pixel->N_Logic(neighbourType, returnPixels);
																						break;
																				case ChunkDirection::NorthEast:
																						result = pixel->NE_Logic(neighbourType, returnPixels);
																						break;
																				case ChunkDirection::East:
																						result = pixel->E_Logic(neighbourType, returnPixels);
																						break;
																				case ChunkDirection::SouthEast:
																						result = pixel->SE_Logic(neighbourType, returnPixels);
																						break;
																				case ChunkDirection::South:
																						result = pixel->S_Logic(neighbourType, returnPixels);
																						break;
																				case ChunkDirection::SouthWest:
																						result = pixel->SW_Logic(neighbourType, returnPixels);
																						break;
																				case ChunkDirection::West:
																						result = pixel->W_Logic(neighbourType, returnPixels);
																						break;
																				case ChunkDirection::NorthWest:
																						result = pixel->NW_Logic(neighbourType, returnPixels);
																						break;
																				}
																				if (result) {
																						if (DEBUG_PrintPixelData) { printf("X: %i\tY:%i\tUPDATE:%i\t(OUT)\n", x, y, DEBUG_FrameCounter); }
																						if (returnPixels[0] != returnPixels[1]) {
																								if (pixelType != returnPixels[0]) {
																										chunks[chunkIndex]->pixels[_localIndex] = worldDataHandler->GetPixelFromType(returnPixels[0])->GetRandomColour();
																								}
																								if (neighbourType != returnPixels[1]) {
																										if (isSameChunk)
																												chunks[chunkIndex]->pixels[_localIndex] = worldDataHandler->GetPixelFromType(returnPixels[1])->GetRandomColour();
																										else
																												neighbourChunks[DirIndex]->pixels[neighbourIndex] = worldDataHandler->GetPixelFromType(returnPixels[1])->GetRandomColour();
																								}
																						}
																						else {
																								if (isSameChunk)
																										std::swap(chunks[chunkIndex]->pixels[_localIndex], chunks[chunkIndex]->pixels[neighbourIndex]);
																								else
																										std::swap(chunks[chunkIndex]->pixels[_localIndex], neighbourChunks[DirIndex]->pixels[neighbourIndex]);
																						}
																						if (isSameChunk)
																								isProcessed[neighbourIndex] = true;
																						break;
																				}
																		}
																}
														}
												}
												// End
												threadPoolTasks--;
										});
								chunkUpdates++;
						}
				}
				while (threadPoolTasks > 0) {
						Sleep(1);
				}
		}

		//TODO look into doing this in 1 chunk/update. 
		for (int i = 0; i < chunkUpdates; i++)
		{
				memset(isProcessedQueue[i], false, (CHUNK_SIZE_X * CHUNK_SIZE_Y));
		}
}

// Is this really helpful? or just in the way..
//TODO Clean this up a bit?
void WorldSimulator::GetStartAndToForLoop(const short& side, short& xStart, short& xTo, short& yStart, short& yTo) {
		switch (side)
		{
		// case ChunkDirection::North:
		case 0:
				yStart = 0;
				yTo = (yDir == 1 ? 1 : -1);
				goto jumpToSetX;
				// case ChunkDirection::South:
		case 1:
				yStart = CHUNK_SIZE_Y - 1;
				yTo = (yDir == 1 ? CHUNK_SIZE_Y : CHUNK_SIZE_Y - 2);
				// Set Common X
				jumpToSetX:
				xStart = (xDir == 1 ? 0 : CHUNK_SIZE_X - 1);
				xTo = (xDir == 1 ? CHUNK_SIZE_X - 1: 0);
				break;

		// case ChunkDirection::East:
		case 2:
				xStart = 0;
				xTo = (xDir == 1 ? 1 : -1);
				goto jumpToSetY;
				break;
		// case ChunkDirection::West:
		case 3:
				xStart = CHUNK_SIZE_X - 1;
				xTo = (xDir == 1 ? CHUNK_SIZE_X : CHUNK_SIZE_X - 2);
				// Set Common Y
		jumpToSetY:
				yStart = (yDir == 1 ? 0 : CHUNK_SIZE_Y - 1);
				yTo = (yDir == 1 ? CHUNK_SIZE_Y - 1 : 0);
				break;
		}
}

void WorldSimulator::UpdateInput()
{
		InputManager* input = InputManager::Instance();
		IVec2 mousePos = input->MousePosition();

		if (input->GetMouseButton(MouseLeft)) {
				Pen(mousePos, gameSettings->_paintManager->SelectedPixel, DEBUG_PenSize);
		}
		if (input->GetMouseButton(MouseRight)) {
				Pen(mousePos, worldDataHandler->GetPixelFromIndex(0), DEBUG_PenSize);
		}

		if (input->GetKeyDown(KeyCode::J)) {
				DEBUG_PenSize++;
		}
		if (input->GetKeyDown(KeyCode::K)) {
				DEBUG_PenSize--;
				if (DEBUG_PenSize <= 0) {
						DEBUG_PenSize = 1;
				}
		}

		if (input->GetKeyDown(KeyCode::D)) {
				DEBUG_DrawChunkLines = !DEBUG_DrawChunkLines;
		}
		if (input->GetKeyDown(KeyCode::Space)) {
				DEBUG_DropSand = !DEBUG_DropSand;
		}
		if (input->GetKeyDown(KeyCode::M)) {
				DEBUG_SandDropRate += 5;
		}
		if (input->GetKeyDown(KeyCode::N)) {
				if (DEBUG_SandDropRate > 1) {
						DEBUG_SandDropRate--;
				}
		}
		if (input->GetKeyDown(KeyCode::Up)) {
				DEBUG_PrintPixelData = !DEBUG_PrintPixelData;
		}
		if (input->GetKeyDown(KeyCode::Period)) {
				Pen(mousePos, worldDataHandler->GetPixelFromType(E_PixelType::Water), 1);
		}
		if (input->GetKeyDown(KeyCode::Comma)) {
				Pen(gameSettings->VirtualMouse, worldDataHandler->GetPixelFromType(E_PixelType::Water), 1);
		}
		//TODO Editing Settings, bad
		if (input->GetKeyDown(KeyCode::F)) {
				gameSettings->VirtualMouse.x -= 1 * (input->GetKeyButton(KeyCode::L_Shift) ? 5 : 1);
		}
		if (input->GetKeyDown(KeyCode::H)) {
				gameSettings->VirtualMouse.x += 1 * (input->GetKeyButton(KeyCode::L_Shift) ? 5 : 1);
		}
		if (input->GetKeyDown(KeyCode::T)) {
				gameSettings->VirtualMouse.y-= 1 * (input->GetKeyButton(KeyCode::L_Shift) ? 5 : 1);
		}
		if (input->GetKeyDown(KeyCode::G)) {
				gameSettings->VirtualMouse.y += 1 * (input->GetKeyButton(KeyCode::L_Shift) ? 5 : 1);
		}

		if (input->GetKeyDown(KeyCode::E)) {
				DEBUG_ZOOM += 0.01f;
		}
		else if (input->GetKeyDown(KeyCode::R)) {
				DEBUG_ZOOM += -0.01f;
		}

		//TODO Move this into its own method?
		//? Only debug, a more complete solution that only deletes pixels of type would be cool
		if (input->GetKeyDown(KeyCode::Return)) {
				for (int y = 0; y < WORLD_DIMENSIONS.y; y++)
				{
						for (int x = 0; x < WORLD_DIMENSIONS.x; x++)
						{
								memset(chunks[IVec2(x, y)]->pixels, 0, ChunkTotalSize * sizeof(Uint32));
						}
				}
		}
}

bool WorldSimulator::Draw(Camera* camera) {
		// Copy our texture
		SDL_Rect rect;
		rect.w = CHUNK_SIZE_X;
		rect.h = CHUNK_SIZE_Y;

		// Grab our CameraPosition in Pixels
		SDL_Rect cameraPos = camera->viewPort;
		// Calculate the Position of the Camera in the world in Chunks
		Vec2 cameraWorldPosition = Vec2((float)cameraPos.x / CHUNK_SIZE_X, (float)cameraPos.y / CHUNK_SIZE_Y);
		// Same as above, but now we round to floor
		IVec2 cameraChunk = IVec2(cameraWorldPosition.x, cameraWorldPosition.y);
		// Now we remove our position to get our offset
		IVec2 cameraWorldOffset = IVec2((cameraWorldPosition.x - cameraChunk.x) * CHUNK_SIZE_X, (cameraWorldPosition.y - cameraChunk.y) * CHUNK_SIZE_Y);
		//? printf("CameraWorld X: %f, Y: %f\n", cameraWorldPosition.x, cameraWorldPosition.y);
		//? printf("CameraPixelOffset X: %i, Y: %i\n", cameraWorldOffset.x, cameraWorldOffset.y);

		IVec2 ScreenSize = gameSettings->Screen_Size;

		//TODO Fix all this vv
		for (int xVal = 0; xVal < MaxVisibleChunksOnScreen.x; xVal++)
		{
				int XChunk = xVal; // +cameraChunk.x;
				if (XChunk < 0) continue;
				else if (XChunk >= WORLD_DIMENSIONS.x) { xVal = MaxVisibleChunksOnScreen.x; continue; }

				for (int yVal = 0; yVal < MaxVisibleChunksOnScreen.y; yVal++)
				{
						int YChunk = yVal;// cameraChunk.y;
						if (YChunk < 0) continue;
						else if (YChunk >= WORLD_DIMENSIONS.y) { yVal = MaxVisibleChunksOnScreen.y; continue; }

						// We add one just to center the world based on how we're rendering (Bad choice)
						rect.x = ((xVal + 1) * CHUNK_SIZE_X); // +cameraWorldOffset.x;
						rect.y = ((yVal + 1) * CHUNK_SIZE_Y); // +cameraWorldOffset.y;

						// If part of our visible chunk isn't on screen we need to update the texture a different way
						if (rect.x + CHUNK_SIZE_X > MaxRenderBox.x || rect.y + CHUNK_SIZE_Y > MaxRenderBox.y) {
								//printf("We Skipped X:%i Y:%i\n", XChunk, y + cameraChunk.y);
						}
						else {
								// We update the texture using the entire chunk data
								if (SDL_UpdateTexture(worldTexture, &rect, chunks[IVec2(XChunk, YChunk)]->pixels, CHUNK_SIZE_X * sizeof(Uint32))) {
								}
						}
				}
		}

		//TODO Replace this with a better zoom
		SDL_Rect zoomrect = WorldRenderRect;
		zoomrect.w *= DEBUG_ZOOM;
		zoomrect.h *= DEBUG_ZOOM;

		if (SDL_RenderCopy(gameRenderer, worldTexture, &zoomrect, NULL) != 0) {
				printf("WorldRender Failed!\nError:%s\n", SDL_GetError());
				SDL_ClearError();
		}

		//TODO Move or remove this
		SDL_SetRenderDrawColor(gameRenderer, 50, 50, 50, 50);
		if (DEBUG_DrawChunkLines) {
				IVec2 MaxGridSize((ScreenSize.x + (CHUNK_SIZE_X * 2)) / CHUNK_SIZE_X, (ScreenSize.y + (CHUNK_SIZE_Y * 2)) / CHUNK_SIZE_Y);
				for (int x = 1; x < MaxGridSize.x; x++)
				{
						SDL_RenderDrawLine(gameRenderer, (x * CHUNK_SIZE_X) / DEBUG_ZOOM, 0, (x * CHUNK_SIZE_X) / DEBUG_ZOOM, (CHUNK_SIZE_Y * MaxGridSize.y - 1));
				}
				for (int y = 1; y < MaxGridSize.y; y++)
				{
						SDL_RenderDrawLine(gameRenderer, 0, (y * CHUNK_SIZE_Y) / DEBUG_ZOOM,  (CHUNK_SIZE_X * MaxGridSize.x) - 1, (y * CHUNK_SIZE_Y) / DEBUG_ZOOM);
				}
		}
		return true;
}

Uint32* WorldSimulator::returnChunk(int chunkIndex)
{
		return nullptr;
}
