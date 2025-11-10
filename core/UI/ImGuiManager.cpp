#include "tetrapc.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "ImGuiManager.h"

#include "../rendering/Viewport.h"
#include "../Core.h"
#include "../rendering/PointLight.h"
#include "../rendering/Texture2D.h"
#include "../GameObject.h"

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

void ImGuiManager::DrawTexture2D(const Texture2D& texture, int width, int height)
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
	auto font1S = io->Fonts->AddFontFromFileTTF((fontPath + "/arial.ttf").c_str(), 15.0f);
	auto font2S = io->Fonts->AddFontFromFileTTF((fontPath + "/Aldrich.ttf").c_str(), 12.0f);
	auto fontMonoS = io->Fonts->AddFontFromFileTTF((fontPath + "/DMMono.ttf").c_str(), 15.0f);

	auto font1M = io->Fonts->AddFontFromFileTTF((fontPath + "/arial.ttf").c_str(), 20.0f);
	auto font2M = io->Fonts->AddFontFromFileTTF((fontPath + "/Aldrich.ttf").c_str(), 16.0f);
	auto fontMonoM = io->Fonts->AddFontFromFileTTF((fontPath + "/DMMono.ttf").c_str(), 20.0f);

	auto font1L = io->Fonts->AddFontFromFileTTF((fontPath + "/arial.ttf").c_str(), 30.0f);
	auto font2L = io->Fonts->AddFontFromFileTTF((fontPath + "/Aldrich.ttf").c_str(), 24.0f);
	auto fontMonoL = io->Fonts->AddFontFromFileTTF((fontPath + "/DMMono.ttf").c_str(), 30.0f);

	auto font1XL = io->Fonts->AddFontFromFileTTF((fontPath + "/arial.ttf").c_str(), 40.0f);
	auto font2XL = io->Fonts->AddFontFromFileTTF((fontPath + "/Aldrich.ttf").c_str(), 32.0f);
	auto fontMonoXL = io->Fonts->AddFontFromFileTTF((fontPath + "/DMMono.ttf").c_str(), 40.0f);

	io->FontDefault = fontMonoM;


	ImGui_ImplGlfw_InitForOpenGL(Core::glfwManager->window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	hierarchy = std::make_unique<UI::Hierarchy>();
	componentDisplay = std::make_unique<UI::Inspector>();

	SetInspectors();
}

void ImGuiManager::BindEvents() {
	TETRA_USE_MAIN_ECS
	hierarchy->BindEvents();
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
			if (ImGui::MenuItem("Quit", NULL, false, enableDockSpace)) {
				glfwSetWindowShouldClose(GLFWManager::get()->window, true);
				enableDockSpace = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Open Window")) {

			if (ImGui::MenuItem("Edit Style", NULL, false, enableDockSpace)) {
				showStyles = true;
			}
			if (ImGui::MenuItem("Inspector", NULL, false, enableDockSpace)) {
				showComponentDisplay = true;
			}
			if (ImGui::MenuItem("Viewport", NULL, false, enableDockSpace)) {
				showViewport = true;
			}
			if (ImGui::MenuItem("Hierarchy", NULL, false, enableDockSpace)) {
				showHierarchy = true;
			}
			ImGui::EndMenu();

		}
		HelpMarker("Main app space");

		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void ImGuiManager::ShowHierarchy() {
	if (!hierarchy) {
		LOG_ERR("No hierarchy");
		return;
	}

	hierarchy->Draw(&showHierarchy);
}

void ImGuiManager::ShowComponentDisplay() {
	if (!componentDisplay) {
		LOG_ERR("No component display");
		return;
	}
	if (!hierarchy || !showHierarchy) {
		componentDisplay->Display(ECS::Entity(), &showComponentDisplay);
	}
	else {
		componentDisplay->Display(hierarchy->GetSelected(), &showComponentDisplay);
	}
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

void ImGuiManager::SetInspectors() {
	componentDisplay->RegisterComponentDisplay<GameObjectInfo, UI::GameObjectInfoComponentDisplay>();
	componentDisplay->RegisterComponentDisplay<Transform, UI::TransformComponentDisplay>();
	componentDisplay->RegisterComponentDisplay<MeshRenderer, UI::MeshRendererComponentDisplay>();
	componentDisplay->RegisterComponentDisplay<PointLight, UI::PointLightComponentDisplay>();
	componentDisplay->RegisterComponentDisplay<RigidBody, UI::RigidBodyComponentDisplay>();
}

void ImGuiManager::Render()
{

	if (enableDockSpace) ShowDockSpace();
	if (showViewport) ShowViewport(Core::mainViewport);
	if (showHierarchy) ShowHierarchy();
	if (showComponentDisplay) ShowComponentDisplay();

	ImGui::ShowIDStackToolWindow();
	if (showStyles) {
		ImGui::Begin("Style Editor", &showStyles);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	//ImGui::ShowDemoWindow(); // Show demo window! :)

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

void ImGuiManager::StartRender() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::EndRender()
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
