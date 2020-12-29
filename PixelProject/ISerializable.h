#pragma once
#include "IO.h"
#include <cereal/cereal.hpp> // for defer
#include <cereal/details/helpers.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

#include <cereal/archives/json.hpp>

/// <summary>
/// An interface to simplify some of how serialization works. 
/// </summary>
class ISerializable
{
public:
		ISerializable() = default;

		virtual bool StartSave(const char* path) {
			const auto newPath = std::filesystem::path(PixelProject::IO::get_executable_path()).append(path);
				std::ofstream os(newPath);
				Save(cereal::JSONOutputArchive(os));
				return true;
		}

		virtual bool StartLoad(const char* path, const bool on_fail_save = false) {
			const auto newPath = std::filesystem::path(PixelProject::IO::get_executable_path()).append(path);
				if (PixelProject::IO::does_full_path_exist(newPath)) {
						std::ifstream fs(newPath);
						Load(cereal::JSONInputArchive(fs));
						return true;
				}
				if (on_fail_save) {
						return StartSave(path);
				}
				return false;
		}

		virtual std::string FilePath() = 0;

		//TODO Any way to enforce this?
		// Any class that utilizes this class will likely need this implemented.
		//template<class Archive>
		//void serialize(Archive& archive)
		//{
		//		archive(x, y);
		//}

		virtual ~ISerializable() = default;
protected:
		virtual void Save(cereal::JSONOutputArchive out_archive) = 0;
		virtual void Load(cereal::JSONInputArchive in_archive) = 0;
};