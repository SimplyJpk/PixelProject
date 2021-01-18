#pragma once
#include <SDL_filesystem.h>

#include <filesystem>
#include <cstdio>
#include <fstream>

namespace PixelProject::IO
{
   inline std::filesystem::path get_executable_path() { return std::filesystem::path(SDL_GetBasePath()); }

   inline std::filesystem::path get_file_path(const std::string& path)
   {
      return std::filesystem::path(SDL_GetBasePath() + path);
   }

   inline bool does_full_path_exist(const std::filesystem::path& path)
   {
      return std::filesystem::exists(path);
   }

   inline bool does_file_exist(const std::string& file_path, const bool use_sdl_base = true)
   {
      if (use_sdl_base)
      {
         return std::filesystem::exists(std::filesystem::path(SDL_GetBasePath() + file_path));
      }
      return std::filesystem::exists(file_path);
   }

   inline bool does_file_exist(const std::filesystem::path& file_path)
   {
      return std::filesystem::exists(file_path);
   }

   //TODO This can probably be improved to remove or reduce the allocation involved
   inline void create_directory(const std::string& directory)
   {
      char filePath[100]{};
      directory.copy(filePath, directory.find_last_of('/'), 0);
      std::filesystem::create_directories(filePath);
   }

   // Creates an empty file using the argument file, it will check to make sure directory exists prior.
   inline void create_file_if_not_exist(const std::string& file)
   {
      const std::filesystem::path path = SDL_GetBasePath() + file;
      if (does_file_exist(path.string(), false)) { return; }
      // Make sure directory exists
      create_directory(path.string());
      // Create file
      std::ofstream of(file);
      of.close();
   }
}
