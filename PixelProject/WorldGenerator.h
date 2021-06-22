#pragma once
#include <cstdio>

#include <glm/vec2.hpp>
#include "Constants.h"

#include "lib/FastNoiseSIMD/FastNoiseSIMD.h"

#include "PixelTypeIncludes.h"
#include "WorldChunk.h"
#include "WorldDataHandler.h"
#include "WorldChunk.h"

class WorldGenerator
{
public:
   static inline FastNoiseSIMD* noise = nullptr;

   static inline WorldDataHandler* world_data = nullptr;

   static bool GenerateChunk(const glm::vec2& world_position, WorldChunk* world_chunk);

   WorldGenerator();
};

