#include "tetrapc.h"
#include "../resources/SharedContentRegistry.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "../utils/Utils.h"

void SharedContentRegistry::LoadAllAssets() {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(assetPath)) {
        if (entry.is_regular_file()) {
            std::filesystem::path file = entry.path();
            LoadFile(file.string());
        }
    }
}

void SharedContentRegistry::LoadFile(const std::filesystem::path &path) {
    std::filesystem::path metaPath;
    metaPath += ".meta";
    bool hasMeta = std::filesystem::exists(metaPath);
    asset_type_id type;
    GUID id;
    std::ifstream stream(metaPath.string(), std::ifstream::binary);
    if (!stream.is_open()) {
        return;
    }
    nlohmann::json json;
    stream >> json;
    id = Utils::GuidFromString(json["guid"]);
    type = json["type"];
    if (type == TypeInfo<Texture2D>::id) {
        auto ptr = Load<Texture2D>(path);
        ptr.GetMetadataRef().path = path.string();
    }
    std::cout << "File was loaded at " << path << std::endl;
}
