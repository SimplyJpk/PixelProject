#pragma once
#include <gl/GLEW.h>
#include <cstdio>
#include <string>
#include <unordered_map>

#include "IO.h"
#include "Shader.h"

using namespace PixelProject;

enum ShaderMask : uint8_t
{
   MVertex = 1 << 0,
   MFragment = 1 << 1,
   MGeometry = 1 << 2,
   MTessEval = 1 << 3,
   MTessControl = 1 << 4,
   MCompute = 1 << 5,
   BitMaskMax = MCompute
};

enum class ShaderTypes : GLint
{
   Vertex = GL_VERTEX_SHADER,
   Fragment = GL_FRAGMENT_SHADER,
   Geometry = GL_GEOMETRY_SHADER,
   TessEval = GL_TESS_EVALUATION_SHADER,
   TessControl = GL_TESS_CONTROL_SHADER,
   Compute = GL_COMPUTE_SHADER
};

//TODO Way to combine shaders of different names.

//TODO any way to make this only visible to this class?
constexpr short shader_types_count = 6;
class ShaderManager
{
public:
   static ShaderManager& Instance();

   Shader* CreateShaderProgram(const std::string& shader_name, bool delete_sources = true);
   Shader* CreateShaderProgramFromFiles(uint8_t shader_mask, const std::string& name, const std::string fileName);


   bool ShaderFromText(GLenum type, const std::string& name, const char* src);
   bool ShaderFromFile(GLint index, const std::string& name, const std::string fileName);
   bool ShaderFromFile(ShaderTypes type, const std::string& name, const std::string fileName);

   //TODO Implement
   bool ShaderCompiledFile(GLenum type, std::string name, const std::string fileName);

   bool ShaderFromFiles(uint8_t shader_mask, const std::string& name, const std::string fileName);

   Shader& GetShader(GLint program_id);
   Shader& GetShader(std::string program_name);

   GLint GetProgramID(const char* program_name);
   std::string GetProgramName(const GLint program_id);

   inline void UseProgram(const char* program_name);
   static inline void UseProgram(const GLint program_id);

   ShaderManager(const ShaderManager&) = delete;
   ShaderManager(ShaderManager&&) = delete;
   void operator=(const ShaderManager&) = delete;
   void operator=(ShaderManager&&) = delete;

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

private:
   ShaderManager() = default;
   ~ShaderManager() = default;

   GLenum gl_enum_from_index_[6] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER, GL_TESS_EVALUATION_SHADER, GL_TESS_CONTROL_SHADER, GL_COMPUTE_SHADER };

   std::vector<std::string> shader_end_ = { "vert", "frag", "geom", "tesse", "tessc", "comp" };

   std::unordered_map<std::string, GLint> program_id_;
   std::unordered_map<GLint, std::string> program_name_;

   std::map < std::string, Shader*> linked_shaders_;

   // Used to simplify 
   std::unordered_map<std::string, int[shader_types_count]> shader_contents_map_;
};

