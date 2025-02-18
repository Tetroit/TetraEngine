project "glad"
    kind "StaticLib"
    language "C"
    
    targetdir(libpath)
    objdir(objpath .. "/glad")

    includedirs
    {
        "include"
    }
    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c",
    }
    
	filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"