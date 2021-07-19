#pragma once
#include <glm/vec2.hpp>

#include "Shader.h"

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
	 unsigned int fbo_texture_;
	 unsigned int fbo_depth_;

	 unsigned int vao_;
	 unsigned int vbo_;

	 // Size of the Buffer
	 glm::vec2 buffer_size_ = glm::vec2(0, 0);

	 // Texel size
	 glm::vec2 half_texel_ = 1.0f / glm::vec2(1280, 720) * 0.5f;
};
