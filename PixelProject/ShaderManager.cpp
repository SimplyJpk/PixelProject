#include "ShaderManager.h"

ShaderManager& ShaderManager::Instance()
{
   static ShaderManager instance;
   return instance;
}

Shader* ShaderManager::CreateShaderProgram(const std::string& shader_name, bool delete_sources)
{
   const GLuint program = glCreateProgram();
   const auto shaderArray = shader_contents_map_[shader_name];
   int shadersAdded = 0;
   for (int i = 0; i < shader_types_count; i++)
   {
      if (shaderArray[i] != 0)
      {
         glAttachShader(program, shaderArray[i]);
         shadersAdded++;
      }
   }
   // Link our program
   glLinkProgram(program);
   GLint isLinked = 0;
   glGetProgramiv(program, GL_LINK_STATUS, static_cast<int*>(&isLinked));
   if (isLinked == GL_FALSE)
   {
      GLint maxLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

      // Clear Program, and shaders if we're doing that here.
      glDeleteProgram(program);
      //TODO Tidy this with below #1
      if (delete_sources)
      {
         for (int i = 0; i < shader_types_count; i++)
         {
            if (shaderArray[i] != 0)
            {
               glDeleteShader(shaderArray[i]);
            }
         }
         shader_contents_map_.erase(shader_name);
      }
      return nullptr;
   }
   printf("Shader Program '%s' Generated using %i modules\n", shader_name.c_str(), shadersAdded);
   program_id_[shader_name.c_str()] = program;
   program_name_[program] = shader_name;

   // Add to linked Shaders
   Shader* shader = new Shader(program, shader_name);
   linked_shaders_[shader_name] = shader;

   //TODO Tidy this with above #1
   if (delete_sources)
      {
         for (int i = 0; i < shader_types_count; i++)
         {
            if (shaderArray[i] != 0)
            {
               glDeleteShader(shaderArray[i]);
            }
         }
         shader_contents_map_.erase(shader_name);
      }
   return shader;
}

Shader* ShaderManager::CreateShaderProgramFromFiles(uint8_t shader_mask, const std::string& name,
   const std::string fileName)
{
   if (ShaderFromFiles(shader_mask, name, fileName))
   {
      return CreateShaderProgram(name, false);
   }
   printf("Failed to Create shader '%s'\n", name.c_str());
   return nullptr;
}

bool ShaderManager::ShaderFromText(GLenum type, const std::string& name, const char* src)
{
   if (type == 0)
   {
      std::printf("Error creating shader.\n");
      return false;
   }
   GLuint shader = glCreateShader(type);
   const GLchar* codeArray[] = { src };
   glShaderSource(shader, 1, codeArray, nullptr);
   // Compile
   glCompileShader(shader);
   // Check to confirm
   GLint result;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
   if (GL_FALSE == result) {
      printf("Shader compilation failed!\nSource: %s", name.c_str());
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
      shader_contents_map_[name][GetShaderIndex(type)] = shader;
      return true;
   }
   return false;
}

bool ShaderManager::ShaderFromFile(ShaderTypes type, const std::string& name, const std::string fileName)
{
   // Get Shader Code so we can compile it
   const std::string shaderCode = IO::get_file_contents(fileName);
   return ShaderFromText(static_cast<GLenum>(type), name, shaderCode.c_str());
}
bool ShaderManager::ShaderFromFile(GLint index, const std::string& name, const std::string fileName)
{
   // Get Shader Code so we can compile it
   const std::string shaderCode = IO::get_file_contents(fileName);
   return ShaderFromText(gl_enum_from_index_[index], name, shaderCode.c_str());
}

GLint ShaderManager::GetProgramID(const char* program_name)
{
   return program_id_[program_name];
}

std::string ShaderManager::GetProgramName(const GLint program_id)
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

bool ShaderManager::ShaderFromFiles(uint8_t shader_mask, const std::string& name, const std::string fileName)
{
   int shaders_loaded = 0;
   int index = 5;
   int stride = ShaderMask::BitMaskMax;
   // We loop through our possible shaders and load any that we've been asked to load
   while (stride > 0)
   {
      if (stride & shader_mask)
      {
         if (ShaderFromFile(index, name, fileName + "." + shader_end_[index]))
            shaders_loaded++;
      }
      stride >>= 1;
      index--;
   }
   if (shaders_loaded > 0)
      return true;
   return false;
}

Shader& ShaderManager::GetShader(GLint program_id)
{
   std::unordered_map<int, std::string>::iterator i;
   if ((i = program_name_.find(program_id)) != program_name_.end())
   {
      return GetShader(i->second);
   }
   //TODO Do we really want to throw? Maybe we should have some default error shader we can fall on. ie; all purple
   printf("Shader with id '%i' does not exist!\n", program_id);
   throw;
}

Shader& ShaderManager::GetShader(const std::string program_name)
{
   const auto value = linked_shaders_.find(program_name);
   return *value->second;
}
