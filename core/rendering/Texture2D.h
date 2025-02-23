#pragma once
namespace TetraEngine {
	class Texture2D
	{
		uint texture;
		static uint attached;
	public:
		int width;
		int height;
		int channels;
		unsigned char* data;

		Texture2D();
		Texture2D(int width, int height, int channels = 3);
		~Texture2D();

		static void Unbind(int id = 0);
		void Load(const std::string& name, bool flip_vertically = false);
		void Bind(int id = 0);
		bool IsAttached() const;
		void Resize(uint width, uint height);
		uint ID() const;
	};
}

