#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TextureUtility
{
public:
   // Crude Paramater Setter, uses the defaults expected for PixelProject, but can be used to set other values as needed.
   static void SetTexParams(
      const GLenum target = GL_TEXTURE_2D, 
      const GLint wrap_s = GL_REPEAT, 
      const GLint wrap_t = GL_REPEAT, 
      const GLint min_filter = GL_NEAREST, 
      const GLint mag_filter = GL_NEAREST)
   {
      // set the texture wrapping parameters
      glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
      glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
      // set texture filtering parameters
      glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter);
      glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter);
   }
};
