#include "RenderTarget.h"

// Creates a renderTarget using width and height given
RenderTarget::RenderTarget(const unsigned int width, const unsigned int height)
{
	 buffer_size_.x = static_cast<float>(width);
	 buffer_size_.y = static_cast<float>(height);

	 // Build Buffer
	 SetupBuffer(width, height);
	 // Setup Vertex Buffer
	 SetupVertex();
}

void RenderTarget::SetShader(Shader* shader)
{
	 post_shader_ = shader;
}

// Bind the Render Target so we can draw to it, Option to clear as bool, default clears
void RenderTarget::BindRenderTarget(bool clear) const
{
	 // Bind Target
	 glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	 glViewport(0, 0, static_cast<int>(buffer_size_.x), static_cast<int>(buffer_size_.y));

	 // Clear screen/target
	 if (clear)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Draws the quad onto the screen at position x and y using shader provided
void RenderTarget::DrawTargetQuad(const int x, const int y, const bool clear) const
{
	 // Make sure we're not drawing back into our buffer
	 glBindFramebuffer(GL_FRAMEBUFFER, 0);
	 glViewport(x, y, static_cast<int>(buffer_size_.x), static_cast<int>(buffer_size_.y));

	 //x // Use a different shader to the RenderTarget Shader
	 //x post_shader_->UseProgram();
	 //x
	 //x // Set active texture of FBO
	 //x glActiveTexture(GL_TEXTURE0);
	 //x glBindTexture(GL_TEXTURE_2D, fbo_texture_[0]);
	 //x
	 //x // RenderTarget Shader
	 //x int loc = post_shader_->GetUniformLocation("target");
	 //x glUniform1i(loc, 0);
	 //x // Set Kernal
	 //x loc = post_shader_->GetUniformLocation("KernalState");
	 //x glUniform1i(loc, kernal_state);
	 //x
	 //x glBindVertexArray(vao_);
	 //x glDrawArrays(GL_TRIANGLES, 0, 6);
	 //x
	 //x glBindFramebuffer(GL_FRAMEBUFFER, 0);

	 // 2. blur bright fragments with two-pass Gaussian Blur 
	 // --------------------------------------------------
	 bool horizontal = true, first_iteration = true;
	 unsigned int amount = 10;
	 Shader& blurShader = ShaderManager::Instance().GetShader("blur");
	 blurShader.UseProgram();
	 glActiveTexture(GL_TEXTURE0);
	 for (unsigned int i = 0; i < amount; i++)
	 {
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			glUniform1i(blurShader.GetUniformLocation("horizontal"), horizontal);
			glBindTexture(GL_TEXTURE_2D, first_iteration ? fbo_texture_[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)

			glBindVertexArray(vao_);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);

			horizontal = !horizontal;
			if (first_iteration)
				 first_iteration = false;
	 }
	 glBindFramebuffer(GL_FRAMEBUFFER, 0);

	 // Uncomment to view Blur Bloom
	 // glBindTexture(GL_TEXTURE_2D, fbo_texture_[1]);
	 // glBindVertexArray(vao_);
	 // glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	 // glBindVertexArray(0);

	 // 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	 Shader& bloomShader = ShaderManager::Instance().GetShader("bloom");
	 bloomShader.UseProgram();

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, fbo_texture_[0]);
	 glActiveTexture(GL_TEXTURE1);
	 glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
	 // Bloom or not
	 glUniform1i(bloomShader.GetUniformLocation("bloom"), true);

	 // DEBUG for drawing bloom
	 static int status = 0;
	 if (InputManager::Instance().GetKeyDown(KeyCode::_9))
	 {
	    status += 1;
      if (status >= 3)
         status -= 3;
	 }
	 glUniform1i(bloomShader.GetUniformLocation("state"), status);
	 // DEBUG for drawing bloom

	 glBindVertexArray(vao_);
	 glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	 glBindVertexArray(0);
}

void RenderTarget::SetupBuffer(unsigned int width, unsigned int height)
{
	 // setup framebuffer
	 glGenFramebuffers(1, &fbo_);
	 glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

	 // Create Texture and bind it
	 glGenTextures(2, fbo_texture_);

	 for (unsigned int i = 0; i < 2; i++)
	 {
			glBindTexture(GL_TEXTURE_2D, fbo_texture_[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			// attach texture to framebuffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, fbo_texture_[i], 0);
	 }

	 // Setup and bind 24bit depth buffer
	 glGenRenderbuffers(1, &fbo_depth_);
	 glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth_);
	 glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	 // Bind FBO while still bound
	 glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth_);

	 unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	 glDrawBuffers(2, attachments);

	 GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	 if (status != GL_FRAMEBUFFER_COMPLETE)
			printf("Framebuffer Error!\n");

	 glBindFramebuffer(GL_FRAMEBUFFER, 0);

	 // ping-pong-framebuffer for blurring
	 glGenFramebuffers(2, pingpongFBO);
	 glGenTextures(2, pingpongColorbuffers);
	 for (unsigned int i = 0; i < 2; i++)
	 {
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
			glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
			// also check if framebuffers are complete (no need for depth buffer)
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				 printf("pingpong Framebuffer Error!\n");
	 }

}

void RenderTarget::SetupVertex()
{
	 static const float vertexData[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	 };

	 glGenVertexArrays(1, &vao_);
	 glBindVertexArray(vao_);

	 glGenBuffers(1, &vbo_);
	 glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), &vertexData, GL_STATIC_DRAW);

	 // Position
	 glEnableVertexAttribArray(0);
	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	 // TexCoord
	 glEnableVertexAttribArray(1);
	 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	 glBindVertexArray(0);
}
