#pragma once

#include "tetrapc.h"

namespace TetraEngine {
	class Renderbuffer
	{
		uint rbo;
		static uint current;
	public:
		Renderbuffer(uint width, uint height);
		void Resize(uint width, uint height);
		void Bind() const;
		uint ID() const;
		bool IsAttached() const;
		static void Unbind();
	};
}

