#pragma once
#include <gl/GLEW.h>
#include <stb_image.h>
#include <string>

#include "ConsoleUtility.h"
#include "TextureUtility.h"

enum TextureFormat : unsigned int {
   UNKNOWN = 0,
   RED = 1,
   RG,
   RGB,
   RGBA
};

class Texture
{
public:
   // Load an image and use as a texture, should support most general extensions
   Texture(const char* filePath = nullptr)
   {
      int comp;
      unsigned char* texture = stbi_load(filePath, &width_, &height_, &comp, STBI_default);

      if (!texture)
      {
         ConsoleUtility::PrintText("TEXTURE: Failed to load Texture", ConsoleColour::Yellow);
      }
      glGenTextures(1, &texture_id_);
      glBindTexture(GL_TEXTURE_2D, texture_id_);

      switch (comp)
      {
      case STBI_grey:
         format_ = RED;
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width_, height_,
            0, GL_RED, GL_UNSIGNED_BYTE, texture);
         break;
      case STBI_grey_alpha:
         format_ = RG;
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width_, height_,
            0, GL_RG, GL_UNSIGNED_BYTE, texture);
         break;
      case STBI_rgb:
         format_ = RGB;
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_,
            0, GL_RGB, GL_UNSIGNED_BYTE, texture);
         break;
      case STBI_rgb_alpha:
         format_ = RGBA;
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_,
            0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
         break;
      default:
         format_ = UNKNOWN;
         break;
      }

      file_name_ = filePath;

      TextureUtility::SetTexParams(GL_TEXTURE_2D);
   }

   ~Texture()
   {
      if (texture_id_ != 0)
         glDeleteTextures(1, &texture_id_);
      if (loaded_pixels_ != nullptr)
         stbi_image_free(loaded_pixels_);
   }

   GLuint GetHandle()
   {
      return texture_id_;
   }

private:
   GLuint texture_id_;
   int width_;
   int height_;

   unsigned int format_ = TextureFormat::UNKNOWN;
   std::string file_name_;
   unsigned char* loaded_pixels_ = nullptr;
};

