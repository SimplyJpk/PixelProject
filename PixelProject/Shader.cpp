#include "Shader.h"

GLuint Shader::GetProgramID() const
{
   return program_;
}

int Shader::GetUniformLocation(const char* name)
{
   // Check if in map already
   std::map<std::string, GLuint>::iterator i;
   if ((i = _uniformLocations.find(name)) != _uniformLocations.end())
   {
      return i->second;
   }
   // Check if exists in shader
   GLuint pos = glGetUniformLocation(program_, name);
   if (pos != -1)
   {
      _uniformLocations.insert({ name, pos });
      return pos;
   }
   // Complain that it doesn't exist
   //TODO Throw? What is best to handle this?
   printf("Invalid Uniform: '%s' does not exist", name);
   return -1;
}
