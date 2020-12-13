#pragma once
#include "ChunkDirection.h"
#include "Vec2.h"

#include <vector>
#include <SDL.h>

class WorldChunk
{
public:
		IVec2 position;
		WorldChunk* neighbourChunks[ChunkDirection::DIR_COUNT];

		Uint32* pixels = nullptr;

		WorldChunk(IVec2 pos) {
				position = pos;
		}

		/// <summary> Sets the chunks neighbour with zero error checking.</summary>
		void SetNeighbour(ChunkDirection dir, WorldChunk* neighbour) {
				neighbourChunks[dir] = neighbour;
		}
};
