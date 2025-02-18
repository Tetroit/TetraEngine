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

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	libpath = "%{wks.location}/lib"
	objpath = "%{wks.location}/obj"

	extern = {}
	extern["GLFW"] = "%{wks.location}/deps/glfw"
	includepaths = {}
	includepaths["GLFW"] = "%{extern.GLFW}/include"

	group "external"
		include "deps/glfw"
	group ""

	include "core"