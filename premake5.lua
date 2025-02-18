workspace "TetraEngine"

	startproject "TetraEngine"

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

		libpath = "%{wks.location}/lib/%{cfg.buildcfg}"


	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	objpath = "%{wks.location}/obj"

	extern = {}
	extern["GLFW"] = "%{wks.location}/deps/glfw"
	extern["glad"] = "%{wks.location}/deps/glad"
	includepaths = {}
	includepaths["GLFW"] = "%{extern.GLFW}/include"
	includepaths["glad"] = "%{extern.glad}/include"

	group "external"
		include "deps/glfw"
		include "deps/glad"
	group ""

	include "core"