project "TetraEngine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/build/" .. outputdir .. "")
    objdir ("" .. objpath .. "/%{prj.name}/" .. outputdir .. "")

	pchheader ("tetrapc.h")
	pchsource ("pch/tetrapc.cpp")

    includedirs
    {
        "pch",
        "%{includepaths.GLFW}",
        "%{includepaths.glad}",
        "%{includepaths.ImGui}",
        "%{includepaths.glm}",
        "%{includepaths.freetype}",
        "%{includepaths.stb_image}",
        "%{includepaths.assimp}",
    }
    files
    {
        "**.h",
        "**.hpp",
        "**.c",
        "**.cpp",
        "%{extern.stb_image}/*.cpp",
        "%{extern.stb_image}/*.h",
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
    
    defines
    {
        "GLM_ENABLE_EXPERIMENTAL"
    }
	filter "system:windows"
        staticruntime "On"

    filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"
        links
        {
            "assimp-vc143-mtd.lib"
        }

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"
        links
        {
            "assimp-vc143-mt.lib"
        }

    filter "files:%{extern.stb_image}.*cpp"
        flags {"NoPCH"}