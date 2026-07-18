#include "tetrapc.h"
#include "../resources/SharedContentRegistry.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "../utils/Utils.h"

using namespace TetraEngine;

void SharedContentRegistry::LoadAllAssets() {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(assetPath)) {
        if (entry.is_regular_file()) {
            std::filesystem::path file = entry.path();
            LoadFile(file.string());
        }
    }
}

void SharedContentRegistry::LoadFile(const std::filesystem::path &path) {
    if (path.extension() == ".meta") return;
    std::filesystem::path metaPath;
    metaPath += ".meta";
    auto loaded = MetadataLoader::LoadMetadata(metaPath);
    if (!loaded.has_value()) {
        if (path.extension() == ".jpg" || path.extension() == ".jpeg" || path.extension() == ".png") {
            Load<Texture2D>(path.string());
        }
    }
    else {
        if (loaded->type == TypeInfo<Texture2D>::id) {
            auto ptr = Load<Texture2D>(path);
            ptr.GetMetadataRef().path = path.string();
        }
    }
    std::ifstream stream(metaPath.string(), std::ifstream::binary);
    std::cout << "File was loaded at " << path << std::endl;
}
