project "TetraEngine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/build/" .. outputdir .. "")
    objdir ("" .. objpath .. "/%{prj.name}/" .. outputdir .. "")

    includedirs
    {
        "%{includepaths.GLFW}"
    }
    files
    {
        "**.h",
        "**.hpp",
        "**.c",
        "**.cpp"
    }
    links
    {
        "GLFW"
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