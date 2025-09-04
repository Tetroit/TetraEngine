#pragma once

#include <fstream>
#include <string>

namespace TetraEngine {
	class OBJParser
	{
	private:
		OBJParser() = default;
		static std::ifstream stream;

	public:
		static int OBJRead(const std::string& path);
	};
}

