#pragma once
#include <SDL.h>
#include <vector>

namespace ChunkUtility
{
		bool static MovePixel(Uint32* chunk, int from, int to, Uint32* toChunk = nullptr)
		{
				if ((toChunk == nullptr ? chunk[to] : toChunk[to]) == 0) {
						std::swap(chunk[from], (toChunk == nullptr ? chunk[to] : toChunk[to]));
						return true;
				}
				return false;
		}

		/// <summary>
		/// Attempts to move the value in From to To in the same chunk. Returns fall if chunk[to] is anything other than 0.
		//TODO Is this faster? It technically should be, but need to confirm. std::swap itself may be slow.
		/// </summary>
		bool static MovePixelSameChunk(Uint32* chunk, int from, int to)
		{
				if (chunk[to] == 0) {
						std::swap(chunk[from], chunk[to]);
						return true;
				}
				return false;
		}
}