project "TetraEngine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/build/" .. outputdir .. "")
    objdir ("" .. objpath .. "/%{prj.name}/" .. outputdir .. "")

	pchheader ("tetrapc.h")
	pchsource ("tetrapc.cpp")

    includedirs
    {
        "%{includepaths.GLFW}",
        "%{includepaths.glad}",
        "%{includepaths.ImGui}",
        "%{includepaths.glm}",
        "%{includepaths.freetype}",
        "%{includepaths.stb_image}",
    }
    files
    {
        "**.h",
        "**.hpp",
        "**.c",
        "**.cpp",
        "%{extern.stb_image}.*cpp",
        "%{extern.stb_image}.*h",
    }
    libdirs
    {
        libpath
    }
    links
    {
        "opengl32.lib",
        "freetype.lib",
        "GLFW",
        "glad",
        "imgui",
    }
    
	filter "system:windows"
        staticruntime "On"

    filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"

    filter "files:%{extern.stb_image}.*cpp"
        flags {"NoPCH"}