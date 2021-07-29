#include "Shader.h"

Shader::Shader(const GLuint program_id)
{
   program_ = program_id;
}

Shader::Shader(const GLuint program_id, const std::string& name)
{
   program_ = program_id;
   name_ = name;
}

void Shader::UseProgram() const
{
   glUseProgram(program_);
}

GLuint Shader::GetProgramID() const
{
   return program_;
}

GLuint Shader::GetUniformLocation(const std::string& uniform_name)
{
   // Check if in map already
   std::map<std::string, GLuint>::iterator i;
   if ((i = uniform_locations_.find(uniform_name)) != uniform_locations_.end())
   {
      return i->second;
   }
   // Check if exists in shader
   GLuint pos = glGetUniformLocation(program_, uniform_name.c_str());
   if (pos != static_cast<GLuint>(-1))
   {
      uniform_locations_.insert({ uniform_name, pos });
      return pos;
   }
   // Complain that it doesn't exist
   //TODO Throw? What is best to handle this?
   printf("Program [%i '%s']: '%s' does not exist.\n", program_, name_.c_str(), uniform_name.c_str());
   return -1;
}
