#pragma once
#include "imgui.h"
#include "behaviours/UpdateManager.h"

namespace TetraEngine {
	class UpdateManager;
}

namespace TetraEngine {
	class Application
	{
	    bool cursorEnabled = true;
		std::unique_ptr<UpdateManager> updateManager;
	public:
		Application();
		virtual ~Application() = default;
		virtual void Update();
	    virtual void DrawGUI(ImVec2 origin, ImVec2 size);

	    void ToggleCursor(bool state);
	    bool IsCursorEnabled();
	};
}

