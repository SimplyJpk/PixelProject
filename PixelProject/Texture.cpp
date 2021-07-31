#include "Texture.h"

Texture::Texture(const int width, const int height, const TextureFormat format)
{
   width_ = width;
   height_ = height;
   format_ = format;

   glGenTextures(1, &texture_id_);
   glBindTexture(GL_TEXTURE_2D, texture_id_);

   switch (format) {
   case TextureFormat::RED_SMALL:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width_, height_, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
      break;
   case TextureFormat::RED_LARGE:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width_, height_, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
      break;
   case TextureFormat::RG:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width_, height_, 0, GL_RG, GL_UNSIGNED_BYTE, nullptr);
      break;
   case TextureFormat::RGB:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
      break;
   case TextureFormat::RGBA:
   case TextureFormat::UNKNOWN:
   default:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
   }

   TextureUtility::SetTexParams(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, 0);
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

   TextureUtility::SetTexParams(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const
{
   glBindTexture(GL_TEXTURE_2D, texture_id_);
}

template <typename T>
void Texture::UpdateTextureData(T data)
{
   Bind();
   //TODO this has to be the ugliest thing ever
#define SHORT_FORMAT(TYPE) format_data_types[static_cast<int>(format_)][static_cast<int>(TYPE)]

   glTexImage2D(SHORT_FORMAT(TextureFormatData::Target), 0, SHORT_FORMAT(TextureFormatData::InternalFormat), width_, height_, 0, SHORT_FORMAT(TextureFormatData::Format), SHORT_FORMAT(TextureFormatData::Type), data);

#undef SHORT_FORMAT
};
