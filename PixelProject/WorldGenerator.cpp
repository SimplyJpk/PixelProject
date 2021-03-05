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
   float* noiseSet = noise->GetSimplexFractalSet(static_cast<int>(world_position.y), static_cast<int>(world_position.x), 0, Constant::chunk_size_y, Constant::chunk_size_x, 1, 1);
   short index = 0;
   for (int x = 0; x < Constant::chunk_size_x; x++)
   {
      for (int y = 0; y < Constant::chunk_size_y; y++)
      {
         index++;
         float noiseValue = noiseSet[index];

         E_PixelType pixelType = E_PixelType::Space;
         if (noiseValue <= -0.25f)
            pixelType = E_PixelType::Ground;
         else if (noiseValue < 0.0f)
            pixelType = E_PixelType::Water;
         else if (noiseValue < 0.1f)
            pixelType = E_PixelType::Sand;

         Uint32 pixelColour = world_data->GetColorFromType(pixelType);
         pixel_data[index] = pixelColour;
      }
   }

   FastNoiseSIMD::FreeNoiseSet(noiseSet);
   return true;
}
