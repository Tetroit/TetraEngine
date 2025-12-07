#pragma once
namespace TetraEngine {
	class Texture2D
	{
		uint texture;
		static uint attached;
		std::string path;
	public:
		int width;
		int height;
		int channels;
		unsigned char* data;

		Texture2D();
		Texture2D(int width, int height, int channels = 3);
		explicit Texture2D(const std::string& name, bool flip_vertically = false);
		~Texture2D();

		static void Unbind(int id = 0);
		void Load(const std::string& name, bool flip_vertically = false);
		void Bind(int id = 0);
		bool IsAttached() const;
		void Resize(uint width, uint height);
		[[nodiscard]]
		std::string GetPath() const;
		uint ID() const;
	};
}

