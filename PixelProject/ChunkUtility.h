#pragma once
#include <SDL.h>
#include <vector>

namespace PixelProject::ChunkUtility
{
		void static move_pixel(Uint32* chunk, const int from, const int to, Uint32* to_chunk = nullptr)
		{
				std::swap(chunk[from], (to_chunk == nullptr ? chunk[to] : to_chunk[to]));
		}

		/// <summary>
		/// Attempts to move the value in From to To in the same chunk. Returns fall if chunk[to] is anything other than 0.
		//TODO Is this faster? It technically should be, but need to confirm. std::swap itself may be slow.
		/// </summary>
		void static move_pixel_same_chunk(Uint32* chunk, const int from, const int to)
		{
				std::swap(chunk[from], chunk[to]);
		}
}