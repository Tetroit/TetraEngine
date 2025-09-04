#include "tetrapc.h"
#include "ImGuiManager.h"
#include "rendering/Viewport.h"
#include "Core.h"
#include "rendering/Texture2D.h"

using namespace TetraEngine;

void ImGuiManager::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void ImGuiManager::DrawTexture2D(const Texture2D& texture, int width = -1, int height = -1)
{
	if (width == -1)
		width = texture.width;
	if (height == -1)
		height = texture.height;
	ImGui::Image(texture.ID(), ImVec2(width, height),ImVec2(0,1), ImVec2(1, 0));
}

ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	io = &ImGui::GetIO(); (void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(Core::glfwManager->window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
	
	testTex = new Texture2D();
	testTex->Load(assetPath + "/ship/diffuse.png", true);
}
void ImGuiManager::ShowDockSpace()
{
	bool opt_fullscreen = true;
	bool opt_padding = false;
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoTitleBar;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::Begin("Dockspace", &enableDockSpace, window_flags);
	if (io->ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Quit", NULL, false, enableDockSpace))
			{
				glfwSetWindowShouldClose(GLFWManager::get()->window, true);
				enableDockSpace = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("ImageTest")) {

			DrawTexture2D(*testTex, 1280, 720);
			ImGui::EndMenu();
		}
		HelpMarker("Main app space");

		ImGui::EndMenuBar();
	}
	ImGui::End();
}
void ImGuiManager::ShowViewport(Viewport* vp)
{
	if (ImGui::Begin("Viewport", &showViewport))
	{
		ImVec2 avail_size = ImGui::GetContentRegionAvail();
		vp->SetSize(avail_size.x, avail_size.y);
		DrawTexture2D(*vp->GetTexture(), avail_size.x, avail_size.y);
		if (!allowSceneInteraction)
			allowSceneInteraction = ImGui::IsItemHovered();
	}
	ImGui::End();
}
void ImGuiManager::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	if (enableDockSpace) ShowDockSpace();
	if (showViewport) ShowViewport(Core::mainViewport);
	ImGui::ShowDemoWindow(); // Show demo window! :)

	if ((io->WantCaptureKeyboard || io->WantTextInput) 
		&& !allowSceneInteraction) {
		if (GLFWManager::get()->sendKeyboardEvents)
			GLFWManager::get()->ToggleKeyboardEvents(false);
	}
	else {
		if (!GLFWManager::get()->sendKeyboardEvents)
			GLFWManager::get()->ToggleKeyboardEvents(true);
	}
	if ((io->WantCaptureMouse) 
		&& !allowSceneInteraction)
	{
		if (GLFWManager::get()->sendMouseClickEvents)
			GLFWManager::get()->ToggleMouseClickEvents(false);
		if (GLFWManager::get()->sendMouseMoveEvents)
			GLFWManager::get()->ToggleMouseMoveEvents(false);
	}
	else
	{
		if (!GLFWManager::get()->sendMouseClickEvents)
			GLFWManager::get()->ToggleMouseClickEvents(true);
		if (!GLFWManager::get()->sendMouseMoveEvents)
			GLFWManager::get()->ToggleMouseMoveEvents(true);
	}
}

void ImGuiManager::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

ImGuiManager::~ImGuiManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	io = nullptr;
}
