#pragma once
#include <SDL.h>
#include "Math.h"

#include <vector>

#include "ISerializable.h"
#include "Constants.h"

#include "ChunkSupport.h"

using namespace PixelProject;

class WorldChunk final : public ISerializable
{
public:
   IVec2 position;
   WorldChunk* neighbour_chunks[DIR_COUNT] = {nullptr};

   Uint32 pixel_data[Constant::chunk_total_size] = {0};

   explicit WorldChunk(const IVec2& pos)
   {
      position = pos;
   }

   /// <summary> Sets the chunks neighbour with zero error checking.</summary>
   void SetNeighbour(const E_ChunkDirection dir, WorldChunk* neighbour)
   {
      neighbour_chunks[dir] = neighbour;
   }

   SaveTypes SaveType() override
   {
      return SaveTypes::Binary;
   }

   std::string FilePath() override
   {
      char path[100];
      auto pathResult = sprintf_s(path, "worldData_x%i-y%i", position.x, position.y);
      return path;
   }

   // Inherited via ISerializable
   void Save(cereal::BinaryOutputArchive out_archive) override
   {
      out_archive(CEREAL_NVP(position));

      std::vector<Uint32> pixelData(Constant::chunk_total_size);
      std::copy_n(pixel_data, Constant::chunk_total_size, pixelData.begin());

      out_archive(cereal::make_nvp("ChunkPixels", pixelData));
   }

   void Load(cereal::BinaryInputArchive in_archive) override
   {
      in_archive(position);
      std::vector<Uint32> pixelData(Constant::chunk_total_size);
      in_archive(cereal::make_nvp("ChunkPixels", pixelData));

      std::copy(pixelData.begin(), pixelData.end(), pixel_data);
   }
};
