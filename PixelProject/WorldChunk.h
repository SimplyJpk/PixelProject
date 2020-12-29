#pragma once
#include "ChunkDirection.h"
#include "Vec2.h"

#include <vector>
#include <SDL.h>

class WorldChunk
{
public:
		IVec2 position;
		WorldChunk* neighbour_chunks[DIR_COUNT];

		Uint32* pixels = nullptr;

		WorldChunk(const IVec2& pos)
		{
			position = pos;
		}

		/// <summary> Sets the chunks neighbour with zero error checking.</summary>
		void SetNeighbour(const E_ChunkDirection dir, WorldChunk* neighbour) {
				neighbour_chunks[dir] = neighbour;
		}
};
