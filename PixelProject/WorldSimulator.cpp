#include "WorldSimulator.h"
#include <math.h>
#include <map>
#include "Camera.h"
#include "InputHandler.h"

void WorldSimulator::Start()
{
		// Allocate our pixels and processing bools
		ChunkTotalSize = CHUNK_DIMENSIONS.x * CHUNK_DIMENSIONS.y;

		//gameSettings->_CONFIG_SCREEN_SIZE = IVec2(WORLD_DIMENSIONS.x;
		// pixels = new Uint32[WORLD_DIMENSIONS.x, WORLD_DIMENSIONS.y, ChunkTotalSize];
		// isProcessed = new bool[WORLD_DIMENSIONS.x, WORLD_DIMENSIONS.y, ChunkTotalSize];

		// Set our memory values
		isProcessed = new bool[ChunkTotalSize] {false};

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
						newChunk->pixels = new Uint32[ChunkTotalSize]{ 0 };
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
								{
										localChunk->SetNeighbour(NorthWest, chunks[IVec2(x - 1, y - 1)]);
										// North East
										if (x < WORLD_DIMENSIONS.x - 2) localChunk->SetNeighbour(NorthEast, chunks[IVec2(x + 1, y - 1)]);
								}
						}
						// South
						else if (y < WORLD_DIMENSIONS.y - 1) {
								localChunk->SetNeighbour(South, chunks[IVec2(x, y + 1)]);
								// South West
								if (x > 0)
								{
										localChunk->SetNeighbour(SouthWest, chunks[IVec2(x - 1, y + 1)]);
										// South East
										if (x < WORLD_DIMENSIONS.x - 2) localChunk->SetNeighbour(SouthEast, chunks[IVec2(x + 1, y + 1)]);
								}
						}
						// East West
						else {
								if (x > 0) localChunk->SetNeighbour(East, chunks[IVec2(x + 1, y)]);
								else localChunk->SetNeighbour(West, chunks[IVec2(x - 1, y)]);
						}
				}
		}

		// Create our world texture, we use this to render the world chunks.
		worldTexture = SDL_CreateTexture(gameRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, gameSettings->_CONFIG_SCREEN_SIZE.x + (CHUNK_DIMENSIONS.x * 2), gameSettings->_CONFIG_SCREEN_SIZE.y + (CHUNK_DIMENSIONS.y * 2));
		if (worldTexture == NULL) {
				printf("WorldTexture failed to Init\nError:%s\n", SDL_GetError());
				SDL_ClearError();
		}
}

void WorldSimulator::Pen(IVec2 point, int size)
{
		// Grab our CameraPosition in Pixels
		SDL_Rect cameraPos = cam->viewPort;
		// Calculate the Position of the Camera in the world in Chunks
		Vec2 cameraWorldPosition = Vec2((float)cameraPos.x / CHUNK_DIMENSIONS.x, (float)cameraPos.y / CHUNK_DIMENSIONS.y);
		// Same as above, but now we round to floor
		IVec2 cameraChunk = IVec2(cameraWorldPosition.x, cameraWorldPosition.y);
		// Now we remove our position to get our offset
		IVec2 cameraWorldOffset = IVec2((cameraWorldPosition.x - cameraChunk.x) * CHUNK_DIMENSIONS.x, (cameraWorldPosition.y - cameraChunk.y) * CHUNK_DIMENSIONS.y);

		//TODO Fix this? Error based on drawing, likely on Line 111 when we're just passing our CameraChunk in without any additional checks

		//TODO A lot of math, we should be able to simplify this easily enough by storing within scope. 
		//TODO IE: (WORLD_DIM * CHUNK_DIM) This'll likely never change?
		if (point.x >= 0 && point.x < WORLD_DIMENSIONS.x * CHUNK_DIMENSIONS.x) {
				if (point.y >= 0 && point.y <= WORLD_DIMENSIONS.y * CHUNK_DIMENSIONS.y) {
						for (int x = point.x - size; x < point.x + size; x++)
						{
								// We skip calculations if our position is outside the realm of our world.
								if (x < 0 || x > WORLD_DIMENSIONS.x * CHUNK_DIMENSIONS.x - 1) continue;
								// We get the floor of X divided by Size of Chunks
								static int xFloor;
								xFloor = floor(x / CHUNK_DIMENSIONS.x);
								if (xFloor > WORLD_DIMENSIONS.x) continue;
								for (int y = point.y - size; y < point.y + size; y++)
								{
										if (y < 0 || y > WORLD_DIMENSIONS.y * CHUNK_DIMENSIONS.y - 1) continue;
										static int yFloor;
										yFloor = floor(y / CHUNK_DIMENSIONS.y);
										if (yFloor > WORLD_DIMENSIONS.y) continue;

										chunks[IVec2(xFloor + cameraChunk.x, yFloor + cameraChunk.y)]->pixels[((y - (yFloor * CHUNK_DIMENSIONS.y) - cameraWorldOffset.y) * CHUNK_DIMENSIONS.x) + (x - (xFloor * CHUNK_DIMENSIONS.x) - cameraWorldOffset.x)] = green;
										//x pixels[(yFloor * WORLD_DIMENSIONS.x) + xFloor][((y - (yFloor * CHUNK_DIMENSIONS.y)) * CHUNK_DIMENSIONS.x) + (x - (xFloor * CHUNK_DIMENSIONS.x))] = green;
								}
						}
				}
		}
}

void WorldSimulator::Update() {
		int ChunkOrder = rand() % 2;
		for (int x = 0; x < WORLD_DIMENSIONS.x; x++)
		{
				for (int xDim = 0; xDim < CHUNK_DIMENSIONS.x; xDim++)
				{
						if (rand() % 1000 == 0) {
								chunks[IVec2(x, 0)]->pixels[(xDim * 0) + xDim] = blue;
						}
				}
		}

		for (int xChunk = 0; xChunk < WORLD_DIMENSIONS.x; xChunk++)
		{
				for (int yChunk = 0; yChunk < WORLD_DIMENSIONS.y; yChunk++)
				{
						UpdateChunk(IVec2(xChunk, yChunk));
				}
		}
}

void WorldSimulator::SubscribeInputs(InputHandler* inputHandler)
{
		// When user is clicking/holding mouse down, we want to draw
		inputHandler->Subscribe(this, InputEvent::USER_CLICK_DOWN);
}

void WorldSimulator::UpdateInput(const int eventType, const SDL_Event* _event)
{
		switch (eventType)
		{
		case USER_CLICK_DOWN:
				Pen(IVec2(_event->motion.x, _event->motion.y), 5);
				break;
		default:
				break;
		}
}

bool WorldSimulator::Draw(Camera* camera) {
		// Copy our texture
		SDL_Rect rect;
		rect.w = CHUNK_DIMENSIONS.x;
		rect.h = CHUNK_DIMENSIONS.y;

		// Grab our CameraPosition in Pixels
		SDL_Rect cameraPos = camera->viewPort;
		// Calculate the Position of the Camera in the world in Chunks
		Vec2 cameraWorldPosition = Vec2((float)cameraPos.x / CHUNK_DIMENSIONS.x, (float)cameraPos.y / CHUNK_DIMENSIONS.y);
		// Same as above, but now we round to floor
		IVec2 cameraChunk = IVec2(cameraWorldPosition.x, cameraWorldPosition.y);
		// Now we remove our position to get our offset
		IVec2 cameraWorldOffset = IVec2((cameraWorldPosition.x - cameraChunk.x) * CHUNK_DIMENSIONS.x, (cameraWorldPosition.y - cameraChunk.y) * CHUNK_DIMENSIONS.y);
		printf("CameraWorld X: %f, Y: %f\n", cameraWorldPosition.x, cameraWorldPosition.y);
		printf("CameraPixelOffset X: %i, Y: %i\n", cameraWorldOffset.x, cameraWorldOffset.y);

		IVec2 ScreenSize = gameSettings->_CONFIG_SCREEN_SIZE;

		//TODO Fix all this vv
		for (int xVal = 0; xVal < MaxVisibleChunksOnScreen.x; xVal++)
		{
				int XChunk = xVal + cameraChunk.x;
				if (XChunk < 0) continue;
				else if (XChunk >= WORLD_DIMENSIONS.x) { xVal = MaxVisibleChunksOnScreen.x; continue; }

				for (int yVal = 0; yVal < MaxVisibleChunksOnScreen.y; yVal++)
				{
						int YChunk = yVal + cameraChunk.y;
						if (YChunk < 0) continue;
						else if (YChunk >= WORLD_DIMENSIONS.y) { yVal = MaxVisibleChunksOnScreen.y; continue; }

						// We add one just to center the world based on how we're rendering (Bad choice)
						rect.x = ((xVal + 1) * CHUNK_DIMENSIONS.x) + cameraWorldOffset.x;
						rect.y = ((yVal + 1) * CHUNK_DIMENSIONS.y) + cameraWorldOffset.y;

						// If part of our visible chunk isn't on screen we need to update the texture a different way
						if (rect.x + CHUNK_DIMENSIONS.x > MaxRenderBox.x || rect.y + CHUNK_DIMENSIONS.y > MaxRenderBox.y) {
								//printf("We Skipped X:%i Y:%i\n", XChunk, y + cameraChunk.y);
						}
						else {
								// We update the texture using the entire chunk data
								if (SDL_UpdateTexture(worldTexture, &rect, chunks[IVec2(XChunk, YChunk)]->pixels, CHUNK_DIMENSIONS.x * sizeof(Uint32))) {
								}
						}
				}
		}

		///  Old attempt at rendering, doesn't account for world padding
		// for (int x = cameraChunk.x; x < WORLD_DIMENSIONS.x; x++)
		// {
		// 		for (int y = 0; y < WORLD_DIMENSIONS.y; y++)
		// 		{
		// 				rect.x = x * CHUNK_DIMENSIONS.x + (CHUNK_DIMENSIONS.x); // +camera->viewPort.x;
		// 				rect.y = y * CHUNK_DIMENSIONS.y + (CHUNK_DIMENSIONS.y); // +camera->viewPort.y;
		// 
		// 				if (rect.x < 0 || rect.x >= ScreenSize.x) continue;
		// 				if (rect.y < 0 || rect.y >= ScreenSize.y) continue;
		// 
		// 				// If part of our visible chunk isn't on screen we need to update the texture a different way
		// 				if (rect.x + CHUNK_DIMENSIONS.x + (ScreenSize.x / 2) > ScreenSize.x + (CHUNK_DIMENSIONS.x * 2) || rect.y + CHUNK_DIMENSIONS.y + (ScreenSize.y / 2) > ScreenSize.y + (CHUNK_DIMENSIONS.y * 2)) {
		// 				}
		// 				else {
		// 						// We update the texture using the entire chunk data
		// 						if (SDL_UpdateTexture(worldTexture, &rect, chunks[IVec2(x, y)]->pixels, CHUNK_DIMENSIONS.x * sizeof(Uint32))) {
		// 						}
		// 				}
		// 		}
		// }

		if (SDL_RenderCopy(gameRenderer, worldTexture, &WorldRenderRect, NULL) != 0) {
				printf("WorldRender Failed!\nError:%s\n", SDL_GetError());
				SDL_ClearError();
		}

		//TODO Move or remove this
		SDL_SetRenderDrawColor(gameRenderer, 50, 50, 50, 50);
		//SDL_RenderDrawLine(gameRenderer, 0, CHUNK_DIMENSIONS.y, MaxRenderBox.x, CHUNK_DIMENSIONS.y);
		//SDL_RenderDrawLine(gameRenderer, CHUNK_DIMENSIONS.x, 0, CHUNK_DIMENSIONS.x, MaxRenderBox.y);
		IVec2 MaxGridSize((ScreenSize.x + (CHUNK_DIMENSIONS.x * 2)) / CHUNK_DIMENSIONS.x, (ScreenSize.y + (CHUNK_DIMENSIONS.y * 2)) / CHUNK_DIMENSIONS.y);
		for (int x = 0; x < MaxGridSize.x; x ++)
		{
				for (int y = 0; y < MaxGridSize.y; y ++)
				{
						SDL_RenderDrawLine(gameRenderer, 0, (y * CHUNK_DIMENSIONS.y) + cameraWorldOffset.y, (CHUNK_DIMENSIONS.x * MaxGridSize.x), (y * CHUNK_DIMENSIONS.y) + cameraWorldOffset.y);
						SDL_RenderDrawLine(gameRenderer, (x * CHUNK_DIMENSIONS.x) + cameraWorldOffset.x, 0, (x * CHUNK_DIMENSIONS.x) + cameraWorldOffset.x, (CHUNK_DIMENSIONS.y * MaxGridSize.y));
				}
		}

		return true;
}

bool WorldSimulator::UpdateChunk(IVec2 chunkIndex)
{
		//? May need this later?
		int _localChunkIndex = (chunkIndex.y * WORLD_DIMENSIONS.x) + chunkIndex.x;
		//TODO We should pool these so when I chunk this properly we can use only a handful of arrays
		//Now we know our chunk indexes we create a local group to simplify lookup
		Uint32* localPixels = chunks[chunkIndex]->pixels; // [_localChunkIndex] ->pixels;
		//x bool* localProcessed = isProcessed[_localChunkIndex];
		// Clear our processed array
		memset(isProcessed, false, ChunkTotalSize * sizeof(bool));
		for (int y = CHUNK_DIMENSIONS.y - 1; y >= 0; y--)
		{
				for (int x = CHUNK_DIMENSIONS.x - 1; x >= 0; x--)
				{
						// Set our Local Index
						int _localIndex = (y * CHUNK_DIMENSIONS.x) + x;
						int _adjustedIndex;

						if (!isProcessed[_localIndex] && localPixels[_localIndex] != 0) {
								//printf("Wow");
								if (localPixels[_localIndex] == brown) {
										continue;
								}

								// Check if we're not on the bottom
								if (y < CHUNK_DIMENSIONS.y - 1) {
										// Down
										_adjustedIndex = ((y + 1) * CHUNK_DIMENSIONS.x) + x;
										if (isProcessed[_adjustedIndex] = MovePixel(localPixels, _localIndex, _adjustedIndex)) continue;
										// Any left-Bound logic
										if (x > 0) {
												// Down & Left
												_adjustedIndex = ((y + 1) * CHUNK_DIMENSIONS.x) + (x - 1);
												if (isProcessed[_adjustedIndex] = MovePixel(localPixels, _localIndex, _adjustedIndex)) continue;
										}
										else {
												//? IS THIS WORKING?
												//? It isn't, at least not well
												_adjustedIndex = ((y + 1) * CHUNK_DIMENSIONS.x) + (CHUNK_DIMENSIONS.x - 1);
												WorldChunk* neighbourChunk = chunks[IVec2(chunkIndex.x - 1, chunkIndex.y)];
												if (neighbourChunk != NULL)
														MovePixel(localPixels, _localIndex, _adjustedIndex, neighbourChunk->pixels);
										}
										// Any Right-Bound logic
										if (x < CHUNK_DIMENSIONS.x - 1)
										{
												// Down & Right
												_adjustedIndex = ((y + 1) * CHUNK_DIMENSIONS.x) + (x + 1);
												if (isProcessed[_adjustedIndex] = MovePixel(localPixels, _localIndex, _adjustedIndex)) continue;
										}
										else {
												//? IS THIS WORKING?
												_adjustedIndex = ((y)*CHUNK_DIMENSIONS.x) + 1;
												WorldChunk* neighbourChunk = chunks[IVec2(chunkIndex.x + 1, chunkIndex.y)];
												if (neighbourChunk != NULL)
														MovePixel(localPixels, _localIndex, _adjustedIndex, neighbourChunk->pixels);
										}
								}
								else if (chunkIndex.y < WORLD_DIMENSIONS.y - 1) {
										_adjustedIndex = ((0) * CHUNK_DIMENSIONS.x) + x;
										WorldChunk* neighbourChunk = chunks[IVec2(chunkIndex.x, chunkIndex.y + 1)];
										if (neighbourChunk != NULL)
												MovePixel(localPixels, _localIndex, _adjustedIndex, neighbourChunk->pixels);
								}
						}
				}
		}
		return true;
}

Uint32* WorldSimulator::returnChunk(int chunkIndex)
{
		return nullptr;
}

bool WorldSimulator::MovePixel(Uint32* chunk, int from, int to, Uint32* toChunk)
{
		if ((toChunk == nullptr ? chunk[to] : toChunk[to]) == 0) {
				std::swap(chunk[from], (toChunk == nullptr ? chunk[to] : toChunk[to]));
				return true;
		}
		return false;
}
