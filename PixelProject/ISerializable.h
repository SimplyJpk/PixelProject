#pragma once
#include "IO.h"
#include <cereal/cereal.hpp> // for defer
#include <cereal/details/helpers.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>

enum class SaveTypes
{
    XML,
    Binary,
    Json
};

/// <summary>
/// An interface to simplify some of how serialization works. 
/// </summary>
class ISerializable
{
public:
    ISerializable() = default;

    inline static std::string SaveTypeName[]{ ".xml", "", ".json" };

    virtual bool StartSave(const char* path) {
        const auto saveType = SaveType();
        const auto newPath = std::filesystem::path(PixelProject::IO::get_executable_path()).append("data/").append(path).concat(SaveTypeName[static_cast<int>(saveType)]);
        std::ofstream os(newPath);

        switch (saveType)
        {
        case SaveTypes::XML:
            Save(cereal::XMLOutputArchive(os));
            return true;
            break;
        case SaveTypes::Binary:
            Save(cereal::BinaryOutputArchive(os));
            return true;
            break;
        case SaveTypes::Json:
            Save(cereal::JSONOutputArchive(os));
            return true;
            break;
        }
        return false;
    }

    virtual bool StartLoad(const char* path, const bool on_fail_save = false) {
        const auto saveType = SaveType();
        const auto newPath = std::filesystem::path(PixelProject::IO::get_executable_path()).append("data/").append(path).concat(SaveTypeName[static_cast<int>(saveType)]);
        if (PixelProject::IO::does_full_path_exist(newPath)) {
            std::ifstream fs(newPath);
            const auto saveType = SaveType();
            switch (saveType)
            {
            case SaveTypes::XML:
                Load(cereal::XMLInputArchive(fs));
                return true;
                break;
            case SaveTypes::Binary:
                Load(cereal::BinaryInputArchive(fs));
                return true;
                break;
            case SaveTypes::Json:
                Load(cereal::JSONInputArchive(fs));
                return true;
                break;
            }
            return true;
        }
        if (on_fail_save) {
            return StartSave(path);
        }
        return false;
    }

    virtual std::string FilePath() = 0;

    virtual SaveTypes SaveType() { return SaveTypes::Binary; }

    //TODO Any way to enforce this?
    // Any class that utilizes this class will likely need this implemented.
    //template<class Archive>
    //void serialize(Archive& archive)
    //{
    //		archive(x, y);
    //}

    virtual ~ISerializable() = default;
protected:
    virtual void Save(cereal::JSONOutputArchive out_archive) {};
    virtual void Save(cereal::BinaryOutputArchive out_archive) {};
    virtual void Save(cereal::XMLOutputArchive out_archive) {};

    virtual void Load(cereal::JSONInputArchive in_archive) {};
    virtual void Load(cereal::BinaryInputArchive in_archive) {};
    virtual void Load(cereal::XMLInputArchive in_archive) {};
};