#pragma once

#include "tetrapc.h"
#include <imgui.h>

#include "Inspector.h"
#include "Hierarchy.h"

namespace TetraEngine {
	class Texture2D;
	class Viewport;
	class ImGuiManager
	{
	public:
		bool enableDockSpace = true;
		bool showViewport = true;
		bool showHierarchy = true;
		bool showComponentDisplay = true;
		bool showStyles = false;
		bool allowSceneInteraction = false;

		std::unique_ptr<UI::Hierarchy> hierarchy;
		std::unique_ptr<UI::Inspector> componentDisplay;

		static void HelpMarker(const char* desc);
		static void DrawTexture2D(const Texture2D& texture, int width = -1, int height = -1);
		ImGuiIO* io;
		ImGuiManager();
		void BindEvents();

		void ShowDockSpace();
		void ShowHierarchy();
		void ShowComponentDisplay();
		void ShowViewport(Viewport* vp);
		void SetInspectors();

		void Render();
		void StartRender();
		void EndRender();
		~ImGuiManager();
	};
}

