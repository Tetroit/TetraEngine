workspace "TetraEngine"
	configurations
	{
		"Debug",
		"Release"
	}
	platforms
	{
		"Win64"
	}

	filter {"platforms:Win64"}
		system "Windows"
		architecture "x64"
	filter ""


	extern = {}
	extern["GLFW"] = "glfw"

	group "extern"
		include "TetraEngine/extern/ImGui"
		include "TetraEngine/extern/glad"
	group ""
	
	include "core"