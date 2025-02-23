#pragma once

namespace TetraEngine {
	class Texture2D;
	class Renderbuffer;
	class Framebuffer
	{
		unsigned int fbo;
		Texture2D* texture;
		Renderbuffer* renderbuffer;
		uint width;
		uint height;
	public:
		static unsigned int current;

		Framebuffer(uint width, uint height, bool generateNewTexture = false);
		~Framebuffer();
		void Bind();
		void BindTexture(Texture2D* texture, uint attachment = GL_COLOR_ATTACHMENT0);
		void BindRenderbuffer(Renderbuffer* rb);
		Texture2D* GetTexture();
		uint GetWidth() const;
		uint GetHeight() const;
		bool IsAttached();
		void Resize(int w, int h);
		static bool IsComplete();
		static void Unbind();
	};
}

