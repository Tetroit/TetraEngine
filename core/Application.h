#pragma once
#include "imgui.h"

namespace TetraEngine {
	class Application
	{
	private:
	    bool cursorEnabled = true;
	public:
		Application() = default;
		virtual ~Application() = default;
		virtual void Update();
	    virtual void DrawGUI(ImVec2 origin, ImVec2 size);

	    void ToggleCursor(bool state);
	    bool IsCursorEnabled();
	};
}

