#include "ShaderManager.h"

ShaderManager* ShaderManager::instance_ = nullptr;

ShaderManager::ShaderManager() = default;

ShaderManager* ShaderManager::Instance()
{
   if (!instance_)
      instance_ = new ShaderManager();
   return instance_;
}

int ShaderManager::CreateShaderProgram(const char* shader_name, bool delete_sources)
{
   GLuint program = glCreateProgram();
   auto shaderArray = shader_map_[shader_name];
   for (int i = 0; i < shader_types_count; i++)
   {
      if (shaderArray != 0)
      {
         glAttachShader(program, shaderArray[i]);
      }
   }
   // Link our program
   glLinkProgram(program);
   GLint isLinked = 0;
   glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
   if (isLinked == GL_FALSE)
   {
      GLint maxLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

      // Clear Program, and shaders if we're doing that here.
      glDeleteProgram(program);
      if (delete_sources)
      {
         for (int i = 0; i < shader_types_count; i++)
         {
            if (shaderArray != 0)
            {
               glDeleteShader(shaderArray[i]);
            }
         }
         shader_map_.erase(shader_name);
      }
      return - 1;
   }
   program_id_[shader_name] = program;
   program_name_[program] = shader_name;
   return program;

}

GLint ShaderManager::GetProgramID(const char* program_name)
{
   return program_id_[program_name];
}

const char* ShaderManager::GetProgramName(const GLint program_id)
{
   return program_name_[program_id];
}

inline void ShaderManager::UseProgram(const char* program_name)
{
   glUseProgram(program_id_[program_name]);
}
inline void ShaderManager::UseProgram(const GLint program_id)
{
   glUseProgram(program_id);
}

bool ShaderManager::CompileShader(const char* shader_name, const int shader_type, const char* path)
{
   return CompileShader(shader_name, shader_type, std::string(path));
}
bool ShaderManager::CompileShader(const char* shader_name, const int shader_type, const std::string path)
{
   GLuint shader = glCreateShader(shader_type);
   if (0 == shader_type) {
      std::printf("Error creating shader.\n");
      return false;
   }
   // Get Shader Code so we can compile it
   const std::string shaderCode = IO::get_file_contents(path);
   const GLchar* codeArray[] = { shaderCode.c_str() };
   glShaderSource(shader, 1, codeArray, nullptr);
   // Compile
   glCompileShader( shader );
   // Check to confirm
   GLint result;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
   if (GL_FALSE == result) {
      printf("Vertex shader compilation failed!\nSource: %s", path.c_str());
      // Get and print the info log
      GLint logLen;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
      if (logLen > 0) {
         std::string log(logLen, ' ');
         GLsizei written;
         glGetShaderInfoLog(shader, logLen, &written, &log[0]);
         printf("Shader log: %s", log.c_str());
      }
      glDeleteShader(shader);
   }
   else
   {
      shader_map_[shader_name][GetShaderIndex(shader_type)] = shader;

      return true;
   }
   return false;
}
