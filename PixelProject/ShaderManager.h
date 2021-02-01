#pragma once
#include <gl/GLEW.h>
#include <cstdio>
#include <string>
#include <unordered_map>

#include "IO.h"
using namespace PixelProject;

//TODO Way to combine shaders of different names.

//TODO any way to make this only visible to this class?
constexpr short shader_types_count = 6;
class ShaderManager
{
public:
   static ShaderManager* Instance();

   int CreateShaderProgram(const char* shader_name, bool delete_sources = true);

   bool CompileShader(const char* shader_name, const int shader_type, const std::string path);
   bool CompileShader(const char* shader_name, const int shader_type, const char* path);

   GLint GetProgramID(const char* program_name);
   const char* GetProgramName(const GLint program_id);

   inline void UseProgram(const char* program_name);
   inline void UseProgram(const GLint program_id);

   ShaderManager();
   ShaderManager(ShaderManager const&) = default;
   void operator=(ShaderManager const&) const { }

   enum ShaderTypes : GLint
   {
      Vertex = GL_VERTEX_SHADER,
      Fragment = GL_FRAGMENT_SHADER,
      Geometry = GL_GEOMETRY_SHADER,
      TessellationEval = GL_TESS_EVALUATION_SHADER,
      TessellationControl = GL_TESS_CONTROL_SHADER,
      Compute = GL_COMPUTE_SHADER
   };

private:
   // Simple Converter to change a GL_Shader index to local array indexable value.
   static int GetShaderIndex(const GLint gl_shader_type)
   {
      switch (gl_shader_type) {
      case GL_VERTEX_SHADER:
         return 0;
      case GL_FRAGMENT_SHADER:
         return 1;
      case GL_GEOMETRY_SHADER:
         return 2;
      case GL_TESS_EVALUATION_SHADER:
         return 3;
      case GL_TESS_CONTROL_SHADER:
         return 4;
      case GL_COMPUTE_SHADER:
         return 5;
      }
      return -1;
   }

   std::unordered_map<const char*, GLint> program_id_;
   std::unordered_map<GLint, const char*> program_name_;

   // Used to simplify 
   std::unordered_map<const char*, int[shader_types_count]> shader_map_;

   static ShaderManager* instance_;
};

