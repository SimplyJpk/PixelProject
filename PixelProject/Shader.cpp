#include "Shader.h"

Shader::Shader(const GLuint program_id)
{
   program_ = program_id;
}

void Shader::UseProgram() const
{
   glUseProgram(program_);
}

GLuint Shader::GetProgramID() const
{
   return program_;
}

GLuint Shader::GetUniformLocation(const std::string& name)
{
   // Check if in map already
   std::map<std::string, GLuint>::iterator i;
   if ((i = uniform_locations_.find(name)) != uniform_locations_.end())
   {
      return i->second;
   }
   // Check if exists in shader
   GLuint pos = glGetUniformLocation(program_, name.c_str());
   if (pos != static_cast<GLuint>(-1))
   {
      uniform_locations_.insert({ name, pos });
      return pos;
   }
   // Complain that it doesn't exist
   //TODO Throw? What is best to handle this?
   printf("Invalid Uniform: '%s' does not exist", name.c_str());
   return -1;
}
