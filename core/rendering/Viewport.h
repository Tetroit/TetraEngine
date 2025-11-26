#pragma once

#include "tetrapc.h"

namespace TetraEngine {
	class ViewProvider;
}

namespace TetraEngine
{
	class ViewportCamera;
	class Framebuffer;
	class Texture2D;

	class Viewport
	{
	    void MarkDirty();
	    uint width, height;

	public:
		ViewProvider* cam;
		Framebuffer* framebuffer;
	    bool isDirty;

		Viewport(uint width, uint height, ViewProvider* cam);

		static void Unbind(uint width, uint height);
		void Bind();
		uint GetWidth();
		uint GetHeight();
		void SetSize(uint width, uint height);
		void SetCamera(ViewProvider* cam);
		Texture2D* GetTexture();
	};
}

