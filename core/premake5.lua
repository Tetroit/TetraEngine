project "TetraEngine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/build/" .. outputdir .. "")
    objdir ("" .. objpath .. "/%{prj.name}/" .. outputdir .. "")

    includedirs
    {
        "%{includepaths.GLFW}",
        "%{includepaths.glad}",
        "%{includepaths.ImGui}",
        "%{includepaths.glm}",
    }
    files
    {
        "**.h",
        "**.hpp",
        "**.c",
        "**.cpp",
    }
    links
    {
        "opengl32.lib",
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