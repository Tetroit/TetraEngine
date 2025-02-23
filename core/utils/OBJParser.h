#pragma once

#include <fstream>
#include <string>

namespace TetraEngine {
	static class OBJParser
	{
	private:
		static std::ifstream stream;

	public:
		static int OBJRead(const std::string& path);
	};
}

