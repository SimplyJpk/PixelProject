#pragma once
#include <glm/vec2.hpp>

#include "Shader.h"
#include <gl/GLEW.h>


#include "InputManager.h"
#include "ShaderManager.h"
#include "Texture.h"

//TODO Look into glBindMultiTextureEXT more
//TODO Can be used to set textures straight to textureUnit without glActiveTexture
// https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_direct_state_access.txt

class RenderTarget
{
public:
	 // Constructor to initilise a RenderTarget with size provided
	 RenderTarget(unsigned int width, unsigned int height);
	 ~RenderTarget() = default;

	 void SetShader(Shader* shader);

	 // Kernal State of the Shader
	 unsigned int kernal_state = 1;

	 // Bind RenderTarget, can choose not to clear
	 void BindRenderTarget(bool clear = true) const;
	 // Draw The render Target and position it on screen
	 void DrawTargetQuad(int x = 0, int y = 0, bool clear = true) const;
protected:

	 // Setup Buffer with given Width and Height
	 void SetupBuffer(unsigned int width, unsigned int height);
	 // Setup Vertex
	 void SetupVertex();

	 // Shader of the PostProcess
	 Shader* post_shader_ = nullptr;
private:
	 unsigned int fbo_;

	 Texture* blur_textures_[2];
	 unsigned int fbo_texture_[2];
	 unsigned int fbo_depth_;

	 unsigned int vao_;
	 unsigned int vbo_;

	 // create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)
	 unsigned int pingpongFBO[2];
	 unsigned int pingpongColorbuffers[2];

	 // Size of the Buffer
	 glm::vec2 buffer_size_ = glm::vec2(0, 0);

	 // Texel size
	 glm::vec2 half_texel_ = 1.0f / glm::vec2(1280, 720) * 0.5f;
};
