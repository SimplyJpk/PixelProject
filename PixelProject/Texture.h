#pragma once
#include <GL/glew.h>
#include <stb_image.h>
#include <string>

#include "ConsoleUtility.h"
#include "TextureUtility.h"

enum class TextureFormat : unsigned int {
   UNKNOWN = 0,
   RED = 1,
   RG,
   RGB,
   RGBA
};

//TODO All Texture formats using Unsigned Byte? This may complicate if we do something using more than byte format?

class Texture
{
public:
   // Creates an empty texture
   Texture(const int width, const int height, const TextureFormat format);
   // Load an image and use as a texture, should support most general extensions
   Texture(const char* filePath = nullptr);

   ~Texture()
   {
      if (texture_id_ != 0)
         glDeleteTextures(1, &texture_id_);
      if (loaded_pixels_ != nullptr)
         stbi_image_free(loaded_pixels_);
   }

   void Bind() const;

   GLuint GetHandle() const
   {
      return texture_id_;
   }
   int GetHeight() const
   {
      return height_;
   }
   int GetWidth() const
   {
      return width_;
   }
   TextureFormat GetFormat() const
   {
      return static_cast<TextureFormat>(format_);
   }
   std::string GetFileName() const
   {
      return file_name_;
   }


private:
   GLuint texture_id_;
   int width_;
   int height_;

   TextureFormat format_ = TextureFormat::UNKNOWN;
   std::string file_name_;
   unsigned char* loaded_pixels_ = nullptr;
};

