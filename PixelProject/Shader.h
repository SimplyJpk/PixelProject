#pragma once
#include <map>
#include <string>
#include <GL/glew.h>

// https://codereview.stackexchange.com/questions/92924/modern-opengl-glsl-shader-class
// https://github.com/thenumbernine/Shader/search?q=setUniform
// https://thebookofshaders.com/
// https://subscription.packtpub.com/book/game_development/9781789342253/1/ch01lvl1sec16/saving-and-loading-a-shader-binary

class Shader
{
public:
   Shader(GLuint program_id);
   Shader(GLuint program_id, const std::string& name);

   void UseProgram() const;

   GLuint GetProgramID() const;
   GLuint GetUniformLocation(const std::string& uniform_name);
private:
   GLuint program_;
   std::string name_ = "_NO_NAME_";
   std::map<std::string, GLuint> uniform_locations_;
};