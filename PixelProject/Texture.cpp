#include "Texture.h"

Texture::Texture(const int width, const int height, const TextureFormat format)
{
   width_ = width;
   height_ = height;
   if (format == TextureFormat::UNKNOWN)
      format_ = TextureFormat::RGBA;
   else
      format_ = format;

   glGenTextures(1, &texture_id_);
   // Bind and Set Data
   UpdateTextureData(nullptr);

   TextureUtility::SetTexParams(GL_TEXTURE_2D);
}

Texture::Texture(const char* filePath)
{
   int comp;
   unsigned char* texture = stbi_load(filePath, &width_, &height_, &comp, STBI_default);

   if (!texture)
   {
      ConsoleUtility::PrintText("TEXTURE: Failed to load Texture", ConsoleColour::Yellow);
   }
   glGenTextures(1, &texture_id_);
   glBindTexture(GL_TEXTURE_2D, texture_id_);

   //TODO Will this work with my INT?
   //TODO Test this with loading images
   switch (comp)
   {
   case STBI_grey:
      format_ = TextureFormat::RED_SMALL;
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width_, height_,
         0, GL_RED, GL_UNSIGNED_BYTE, texture);
      break;
   case STBI_grey_alpha:
      format_ = TextureFormat::RG;
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width_, height_,
         0, GL_RG, GL_UNSIGNED_BYTE, texture);
      break;
   case STBI_rgb:
      format_ = TextureFormat::RGB;
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_,
         0, GL_RGB, GL_UNSIGNED_BYTE, texture);
      break;
   case STBI_rgb_alpha:
      format_ = TextureFormat::RGBA;
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_,
         0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
      break;
   default:
      format_ = TextureFormat::UNKNOWN;
      break;
   }
   file_name_ = filePath;

   loaded_pixels_ = texture;
   TextureUtility::SetTexParams(GL_TEXTURE_2D);
}

void Texture::Bind() const
{
   glBindTexture(GL_TEXTURE_2D, texture_id_);
}

std::vector<Texture*> Texture::CreateTextures(const int width, const int height, const TextureFormat format, const int count)
{
   std::vector<Texture*> textures;
   for (int i = 0; i < count; i++)
   {
      textures.push_back(new Texture(width, height, format));
   }
   return textures;
}
