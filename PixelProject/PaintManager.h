#pragma once
#include <stb_image.h>
#include <gl/GLEW.h>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "BasePixel.h"
#include "Camera.h"
#include "GameSettings.h"
#include "WorldDataHandler.h"
#include "InputManager.h"
#include "ShaderManager.h"
#include "Sprite.h"
#include "TextureUtility.h"

//TODO Namespace for this?
constexpr int8_t pixel_texture_size = 32;

class PaintManager
{
public:
   short selected_pixel_type = 0;
   BasePixel* selected_pixel = nullptr;

   const short texture_count = static_cast<int>(E_PixelType::COUNT);
   //? SDL_Texture* pixel_texture[static_cast<int>(E_PixelType::COUNT)];

   //? const Uint32* GetPixelTexture(const short index) const { return pixel_icons[index]; }

   std::string SelectedPixelName() const { return selected_pixel->Name(); }

   void UpdateInput()
   {
      auto* input = InputManager::Instance();
      if (input->GetKeyDown(KeyCode::Tab))
      {
         if (selected_pixel->pixel_index == world_data_.PixelTypeCount() - 1)
            selected_pixel = world_data_.GetPixelFromIndex(1);
         else
            selected_pixel = world_data_.GetPixelFromIndex(selected_pixel->pixel_index + 1);
      }

      IVec2 mousePos = InputManager::Instance()->MousePosition();
      // Crude check, we only want to check if we're hovering if we're close to the items
      if (mousePos.y < pixel_texture_size * 2)
      {
         for (short index = 0; index < texture_count; index++)
         {
            Sprite* sprite = &pixel_sprites_[index];
            glm::vec4 bounds = sprite->transform.GetBounds();
            // If we're moused over
            if (mousePos.x > bounds.x && mousePos.x < bounds.z &&
               mousePos.y > bounds.y && mousePos.y < bounds.w)
            {
               selected_pixel = world_data_.GetPixelFromIndex(index);
               break;
            }
         }
      }
   }

   PaintManager()
   {
      //TODO Init this somewhere else, this could force instant ordering problems.
      selected_pixel = world_data_.GetPixelFromIndex(1);

      int size = pixel_texture_size / 2;
      int point = pixel_texture_size / 2;

      int Left = point - size;
      int Right = (Left + size * 2) - 1;
      int Top = point - size;
      int Bottom = (Top + size * 2) - 1;
      float radius = powf(size, 2);

      for (int index = 0; index < texture_count; index++)
      {
         Uint32* textureInfo = new Uint32[pixel_texture_size * pixel_texture_size]{0x00000000};
         BasePixel* pixel = world_data_.GetPixelFromIndex(index);
         // Generate colours for in-game colours, not UI
         pixel->GenerateColours();
         for (int y = Top; y <= Bottom; ++y)
         {
            for (int x = Left; x <= Right; ++x)
            {
               double dist = powf(point - x, 2.0) + powf(point - y, 2.0);
               if (dist <= radius)
               {
                  textureInfo[(y * pixel_texture_size) + x] = pixel->GetRandomColour();
               }
            }
         }
         pixel_sprites_[index].SetSprite(textureInfo);
      }

      glGenVertexArrays(1, &vao_);
      glGenBuffers(1, &vbo_);
      glGenBuffers(1, &ebo_);

      glBindVertexArray(vao_);

      glBindBuffer(GL_ARRAY_BUFFER, vbo_);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_DYNAMIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_DYNAMIC_DRAW);

      // position attribute
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      //x // color attribute
      //x glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
      //x glEnableVertexAttribArray(1);
      // texture coord attribute
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);
   }

   void GeneratePixelTextures(GameSettings* game_settings)
   {
      // Create our shader
      ShaderManager::Instance().ShaderFromFiles(MVertex | MFragment, "orthoUI", "shaders/orthoUI");
      used_shader_ = ShaderManager::Instance().CreateShaderProgram("orthoUI", false);

      //TODO Need to complete some sort of UI background/border/box?

      // Generate Textures for PixelTypes
      glGenTextures(texture_count, pixel_texture_id_);

      for (int i = 0; i < texture_count; i++) {
         glBindTexture(GL_TEXTURE_2D, pixel_texture_id_[i]);
         TextureUtility::SetTexParams();

         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pixel_texture_size, pixel_texture_size, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, pixel_sprites_[i].GetSprite());

         // Setup structure for future use
         pixel_sprites_[i].SetTextureID(pixel_texture_id_[i]);

         //? Is this trash way to do this?
         Transform& trans = pixel_sprites_[i].transform;
         glm::vec3 position = glm::vec3(
            ((pixel_texture_size / 2) * i) + (i * pixel_texture_size * 2) - ((pixel_texture_size * 2) / 2),
            (pixel_texture_size / 2) + (pixel_texture_size * 2) - ((pixel_texture_size * 2) / 2),
            0
         );
         trans.SetPosition(position);
         trans.SetScale(glm::vec3(pixel_texture_size * 2, pixel_texture_size * 2, 1.0f));
      }

      game_settings_ = game_settings;

      //TODO Work out a better way of doing this, should I have multiple cameras?
      //TODO Remove magic numbers please
      // Set view
      projection_transform_ = glm::ortho((float)0, (float)game_settings->screen_size.x, (float)game_settings->screen_size.y, (float)0);
      //glm::perspective(1.0472f, game_settings->aspect_ratio, 0.1f, 100.f);
      // view_transform_ = glm::lookAt(glm::vec3(5, 0, 5.0f), glm::vec3(5, 0, 0), glm::vec3(0, 1, 0));

      //? for (int index = 0; index < static_cast<int>(E_PixelType::COUNT); index ++)
      //? {
      //?    pixel_texture[index] = SDL_CreateTexture(renderer, Constant::texture_default_format_transparency, SDL_TEXTUREACCESS_STATIC, pixel_texture_size, pixel_texture_size);
      //?    SDL_SetTextureBlendMode(pixel_texture[index], SDL_BLENDMODE_BLEND);
      //?    SDL_UpdateTexture(pixel_texture[index], nullptr, pixel_icons[index],  pixel_texture_size * sizeof(Uint32));
      //? }
   }

   void DrawPaintGUI(Camera* camera)
   {
      used_shader_->UseProgram();

      if (InputManager::Instance()->GetKeyDown(KeyCode::Y))
      {
         IVec2 mousePos = InputManager::Instance()->MousePosition();
         printf("Wow");
      }

      int modelLoc = used_shader_->GetUniformLocation("model");
      // glActiveTexture(GL_TEXTURE0);
      // glBindTexture(GL_TEXTURE_2D, background_image_id_);
      // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(background_image_.transform.GetModel()));
      int projLoc = used_shader_->GetUniformLocation("projection");
      // glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection_transform_));
      // glBindVertexArray(vao_);
      // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      for (int i = 0; i < texture_count; i++)
      {
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, pixel_texture_id_[i]);

         if (selected_pixel->pixel_index != i)
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pixel_sprites_[i].transform.GetModel()));
         else
         {
            glm::mat4 scaledModel = glm::mat4(pixel_sprites_[i].transform.GetModel());
            scaledModel = glm::scale(scaledModel, glm::vec3(1.5f, 1.5f, 1.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(scaledModel));
         }
         glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection_transform_));

         glBindVertexArray(vao_);
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      }
   }

private:
   GameSettings* game_settings_;

   WorldDataHandler& world_data_ = WorldDataHandler::Instance();

   Sprite background_image_;
   Sprite pixel_sprites_[static_cast<int>(E_PixelType::COUNT)];

   Shader* used_shader_ = nullptr;
   GLuint background_image_id_;
   GLuint pixel_texture_id_[static_cast<int>(E_PixelType::COUNT)];
   //TODO Make some sort of class that generates this so we don't have this junk data all over the place?
   unsigned int vbo_, vao_, ebo_;
   float vertices_[20] = {
      // positions             // texture coords
       0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
       0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
      -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
   };
   unsigned int indices_[6] = {
       0, 1, 3, // first triangle
       1, 2, 3  // second triangle
   };

   glm::mat4 projection_transform_;
};
