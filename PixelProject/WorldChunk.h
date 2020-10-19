#pragma once
#include "ChunkDirectionIndex.h"
#include "Vec2.h"

#include <vector>
#include <SDL.h>

class WorldChunk
{
public:
		IVec2 position;
		WorldChunk* neighbourChunks[ChunkDirectionIndex::DIR_COUNT] = { nullptr };

		Uint32* pixels = nullptr;

		WorldChunk(IVec2 pos) {
				position = pos;
		}

		/// <summary> Sets the chunks neighbour with zero error checking.</summary>
		void SetNeighbour(ChunkDirectionIndex dir, WorldChunk* neighbour) {
				neighbourChunks[dir] = neighbour;
		}
};
