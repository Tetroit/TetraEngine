#pragma once
#include <filesystem>
#include <string>

namespace TetraEngine {
	const std::filesystem::path assetPath("./assets");
	const std::filesystem::path fontPath(assetPath / "fonts");
	const std::filesystem::path meshPath(assetPath / "meshes");
	const std::filesystem::path texturePath(assetPath / "textures");
	const std::filesystem::path shaderPath("./shaders");
}
