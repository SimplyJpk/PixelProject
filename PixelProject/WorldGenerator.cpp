#include "WorldGenerator.h"
using namespace PixelProject;

WorldGenerator::WorldGenerator()
{
   world_data = WorldDataHandler::Instance();
   noise = FastNoiseSIMD::NewFastNoiseSIMD();
   noise->SetNoiseType(FastNoiseSIMD::SimplexFractal);
   noise->SetFrequency(0.025f);

   noise->SetFractalType(FastNoiseSIMD::FBM);
   noise->SetFractalOctaves(4);
   noise->SetFractalLacunarity(0.5f);
   noise->SetFractalGain(0.1f);
}

bool WorldGenerator::GenerateChunk(const glm::vec2& world_position, Uint32* pixel_data)
{
   auto* noiseSet = noise->GetSimplexFractalSet(static_cast<int>(world_position.y), static_cast<int>(world_position.x), 0, Constant::chunk_size_y, Constant::chunk_size_x, 1, 1);
   short index = 0;
   for (short x = 0; x < Constant::chunk_size_x; x++)
   {
      for (short y = 0; y < Constant::chunk_size_y; y++)
      {
         index++;
         const auto noiseValue = noiseSet[index];

         auto pixelType = E_PixelType::Space;
         if (noiseValue <= -0.25f)
            pixelType = E_PixelType::Ground;
         else if (noiseValue < 0.0f)
            pixelType = E_PixelType::Water;
         else if (noiseValue < 0.1f)
            pixelType = E_PixelType::Sand;
         else if (noiseValue > 0.5f && noiseValue < 0.54f)
            pixelType = E_PixelType::Gold;

         const auto pixelColour = world_data->GetColorFromType(pixelType);
         pixel_data[index] = pixelColour;
      }
   }

   FastNoiseSIMD::FreeNoiseSet(noiseSet);
   return true;
}
