#pragma once
#include <SDL_filesystem.h>

#include <filesystem>
#include <cstdio>
#include <fstream>

namespace PixelProject::IO
{
		bool DoesFileExist(std::string filepath) {
				return std::filesystem::exists(filepath);
		}

		//TODO This can probably be improved to remove or reduce the allocation involved
		void CreateDirectory(std::string directory) {
				char filepath[100]{};
				directory.copy(filepath, directory.find_last_of('/'), 0);
				std::filesystem::create_directories(filepath);
		}

		// Creates an empty file using the argument file, it will check to make sure directory exists prior.
		void CreateFileIfNotExist(std::string file) {
				if (DoesFileExist(file)) { return; }
				// Make sure directory exists
				CreateDirectory(file);
				// Create file
				std::ofstream of(file);
				of.close();
		}
}