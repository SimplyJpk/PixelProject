#pragma once
#include <GL/glew.h>

enum class TextureFormat : unsigned int {
   UNKNOWN = 0,
   RED_SMALL = 1,
   // Dsd
   RED_LARGE,
   RG,
   RGB,
   RGBA,
   TYPE_COUNT
};

enum class TextureFormatData : unsigned int
{
   Target = 0,
   InternalFormat = 1,
   Format = 2,
   Type = 3,
   VAR_COUNT = 4,
};

namespace Rendering::TextureData
{
   //TODO Look into naming convensions for defines?
   //TODO Look into a better way to organise this data?
   #define SHORT_TYPE constexpr static GLuint

   typedef TextureFormatData TFD;
   #define SHORT_V static_cast<int>(TFD::VAR_COUNT)

   SHORT_TYPE RED_SMALL[SHORT_V] = { GL_TEXTURE_2D, GL_RED, GL_RED, GL_UNSIGNED_BYTE };
   SHORT_TYPE RED_LARGE[SHORT_V] = { GL_TEXTURE_2D, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT };
   SHORT_TYPE RG[SHORT_V] = { GL_TEXTURE_2D, GL_RG, GL_RG, GL_UNSIGNED_BYTE };
   SHORT_TYPE RGB[SHORT_V] = { GL_TEXTURE_2D, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE };
   SHORT_TYPE RGBA[SHORT_V] = { GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE };

#undef SHORT_TYPE
#undef SHORT_V
}

constexpr static GLuint format_data_types[static_cast<int>(TextureFormat::TYPE_COUNT)][static_cast<int>(TextureFormatData::VAR_COUNT)] =
{
   { *Rendering::TextureData::RED_SMALL },
   { *Rendering::TextureData::RED_LARGE},
   { *Rendering::TextureData::RG },
   { *Rendering::TextureData::RGB },
   { *Rendering::TextureData::RGBA }
};