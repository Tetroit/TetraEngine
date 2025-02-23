#pragma once

namespace TetraEngine {
	class Cubemap
	{
	public:
		Cubemap();
		int width;
		int height;
		int channels;
		unsigned int texture;
		unsigned char* data;
		void Load(const std::string& name, const std::string& ext = "png");
		void Bind(int id = 0);
		void Unbind(int id = 0);
	};
}
