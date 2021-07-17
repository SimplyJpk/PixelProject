#include "RenderTarget.h"

#include <gl/GLEW.h>

// Creates a renderTarget using width and height given
RenderTarget::RenderTarget(const unsigned int width, const unsigned int height)
{
	 buffer_size_.x = static_cast<float>(width);
	 buffer_size_.y = static_cast<float>(height);

	 // Build Buffer
	 SetupBuffer(width, height);
	 // Setup Vertex Buffer
	 SetupVertex();

	 // m_PostShader = new Shader("./shaders/rendertarget/vertex.shader", "./shaders/rendertarget/fragment.shader");
}

// Bind the Render Target so we can draw to it, Option to clear as bool, default clears
void RenderTarget::BindRenderTarget(bool clear) const
{
	 // Bind Target
	 glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	 glViewport(0, 0, static_cast<int>(buffer_size_.x), static_cast<int>(buffer_size_.y));

	 // Clear screen/target
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Draws the quad onto the screen at position x and y using shader provided
void RenderTarget::DrawTargetQuad(const int x, const int y, const bool clear) const
{
	 // Make sure we're not drawing back into our buffer
	 glBindFramebuffer(GL_FRAMEBUFFER, 0);
	 glViewport(x, y, static_cast<int>(buffer_size_.x), static_cast<int>(buffer_size_.y));
	 // Clear the depth buffer
	 if (clear)
			glClear(GL_DEPTH_BUFFER_BIT);

	 // Use a different shader to the RenderTarget Shader
	 glUseProgram(m_PostShader->GetProgramID());
	 // Set active texture of FBO
	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, fbo_texture_);

	 // RenderTarget Shader
	 int loc = glGetUniformLocation(m_PostShader->GetProgramID(), "target");
	 glUniform1i(loc, 0);
	 // Set Kernal
	 loc = glGetUniformLocation(m_PostShader->GetProgramID(), "KernalState");
	 glUniform1i(loc, kernal_state_);

	 glBindVertexArray(vao_);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
}

void RenderTarget::SetupBuffer(unsigned int width, unsigned int height)
{
	 // setup framebuffer
	 glGenFramebuffers(1, &fbo_);
	 glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

	 // Create Texture and bind it
	 glGenTextures(1, &fbo_texture_);
	 glBindTexture(GL_TEXTURE_2D, fbo_texture_);
	 // Texture format
	 glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	 // Attach it to framebuffer as first colour attachment
	 glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbo_texture_, 0);

	 // Setup and bind 24bit depth buffer
	 glGenRenderbuffers(1, &fbo_depth_);
	 glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth_);
	 glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

	 // Bind FBO while still bound
	 glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth_);

	 GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	 glDrawBuffers(1, drawBuffers);

	 GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	 if (status != GL_FRAMEBUFFER_COMPLETE)
			printf("Framebuffer Error!\n");

	 glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::SetupVertex()
{
	 half_texel_ = 1.0f / glm::vec2(buffer_size_.x, buffer_size_.y) * 0.5f;

	 // Setup a square
	 float vertexData[] = {
		 -1, -1, 0, 1,	(half_texel_.x),			(half_texel_.y),
			1, 1, 0, 1,	(1 - half_texel_.x),		(1 - half_texel_.y),
		 -1, 1, 0, 1,	(half_texel_.x),			(1 - half_texel_.y),

		 -1, -1, 0, 1,	(half_texel_.x),			(half_texel_.y),
		 1, -1, 0, 1,	(1 - half_texel_.x),		(half_texel_.y),
		 1, 1, 0, 1,		(1 - half_texel_.x),		(1 - half_texel_.y) };

	 glGenVertexArrays(1, &vao_);
	 glBindVertexArray(vao_);

	 glGenBuffers(1, &vbo_);
	 glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertexData, GL_STATIC_DRAW);

	 // Position
	 glEnableVertexAttribArray(0);
	 glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	 // TexCoord
	 glEnableVertexAttribArray(1);
	 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, static_cast<char*>(0) + 16);

	 glBindVertexArray(0);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);
}
