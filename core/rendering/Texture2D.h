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
		std::vector<byte> data;

		Texture2D();
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D(Texture2D&& other) noexcept;
		Texture2D& operator=(Texture2D&& other) noexcept;

		Texture2D(int width, int height, int channels = 3);
		explicit Texture2D(const std::string& name, bool flip_vertically = false);
		~Texture2D();

		static void Unbind(int id = 0);
		void Load(const std::filesystem::path& name, bool flip_vertically = false) {
			Load(name.string(), flip_vertically);
		}
		void Load(const std::string& name, bool flip_vertically = false);
		void Bind(int id = 0);
		bool IsAttached() const;
		void Resize(uint width, uint height);
		[[nodiscard]]
		std::string GetPath() const;
		uint ID() const;
	};
}

