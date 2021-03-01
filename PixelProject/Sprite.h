#pragma once
#include <SDL_stdinc.h>
#include <glm/ext/matrix_transform.hpp>
#include <GL\glew.h>

#include "Transform.h"

class Sprite
{
public:
   Transform transform;

   GLuint GetTextureID() const { return texture_id_; }
   Uint32* GetSprite() { return pixel_icon_; }

   // Setters
   void SetTextureID(GLuint id) { texture_id_ = id; }
   void SetSprite(Uint32* sprite) { pixel_icon_ = sprite; }
protected:
   GLuint texture_id_ = -1;
   Uint32* pixel_icon_ = nullptr;
};
