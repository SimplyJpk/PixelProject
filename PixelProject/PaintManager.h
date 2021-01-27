#pragma once
#include "BasePixel.h"
#include "WorldDataHandler.h"
#include "InputManager.h"

//TODO Namespace for this?
constexpr int8_t pixel_texture_size = 32;

class PaintManager
{
public:
   short selected_pixel_type = 0;
   BasePixel* selected_pixel = nullptr;

   Uint32 pixel_icons[static_cast<int>(E_PixelType::COUNT)][pixel_texture_size * pixel_texture_size] = {0};
   SDL_Texture* pixel_texture[static_cast<int>(E_PixelType::COUNT)];

   const Uint32* GetPixelTexture(const short index) const { return pixel_icons[index]; }

   const char* SelectedPixelName() const { return selected_pixel->Name(); }

   void UpdateInput()
   {
      auto* input = InputManager::Instance();
      if (input->GetKeyDown(KeyCode::Tab))
      {
         if (selected_pixel->pixel_index == world_data_->PixelTypeCount() - 1)
            selected_pixel = world_data_->GetPixelFromIndex(1);
         else
            selected_pixel = world_data_->GetPixelFromIndex(selected_pixel->pixel_index + 1);
      }
   }

   PaintManager()
   {
      world_data_ = WorldDataHandler::Instance();
      //TODO Init this somewhere else, this could force instant ordering problems.
      selected_pixel = world_data_->GetPixelFromIndex(1);

      int size = pixel_texture_size / 2;
      int point = pixel_texture_size / 2;

      int Left = point - size;
      int Right = (Left + size * 2) - 1;
      int Top = point - size;
      int Bottom = (Top + size * 2) - 1;
      float radius = powf(size, 2);

      for (int index = 0; index < static_cast<int>(E_PixelType::COUNT); index++)
      {
         BasePixel* pixel = world_data_->GetPixelFromIndex(index);
         for (int y = Top; y <= Bottom; ++y)
         {
            for (int x = Left; x <= Right; ++x)
            {
               double dist = powf(point - x, 2.0) + powf(point - y, 2.0);
               if (dist <= radius)
               {
                  pixel_icons[index][(y * pixel_texture_size) + x] = 0xFF000000 | pixel->GetRandomColour();
               }
            }
         }
      }
   }

   void GeneratePixelTextures(SDL_Renderer* renderer)
   {
      for (int index = 0; index < static_cast<int>(E_PixelType::COUNT); index ++)
      {
         pixel_texture[index] = SDL_CreateTexture(renderer, Constant::texture_default_format_transparency, SDL_TEXTUREACCESS_STATIC, pixel_texture_size, pixel_texture_size);
         SDL_SetTextureBlendMode(pixel_texture[index], SDL_BLENDMODE_BLEND);
         SDL_UpdateTexture(pixel_texture[index], nullptr, pixel_icons[index],  pixel_texture_size * sizeof(Uint32));
      }
   }

   void DrawTexture(SDL_Renderer* renderer, const IVec2 position, E_PixelType pixel_type)
   {
      int index;
      if (pixel_type == E_PixelType::COUNT)
         index = selected_pixel->pixel_index;
      else
         index = static_cast<int>(pixel_type);

      SDL_Rect rect = {position.x, position.y, pixel_texture_size, pixel_texture_size};
      SDL_RenderCopy(renderer, pixel_texture[index], nullptr, &rect);
   }

private:
   WorldDataHandler* world_data_ = nullptr;
};
