#pragma once
#include <cstdio>

#include <glm/vec2.hpp>
#include "Constants.h"

#include "lib/FastNoiseSIMD/FastNoiseSIMD.h"

#include "PixelTypeIncludes.h"
#include "WorldDataHandler.h"

class WorldGenerator
{
public:
   FastNoiseSIMD* noise = nullptr;

   WorldDataHandler* world_data;

   bool GenerateChunk(const glm::vec2& world_position, Uint32* pixel_data);

   WorldGenerator();
};

