project "Polonium"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir "bin/%{cfg.buildcfg}"
    objdir    "bin/obj/%{cfg.buildcfg}"
	
	pchheader "plpch.h"
    pchsource "pch/plpch.cpp"
	
	files{
		"**.cpp",
		"**.hpp",
		"**.h",
	}
	
	includedirs{
		"include",
		"../include",
		"../include/FREETYPE",
		"pch",
	}
	
	libdirs {
        "../libs/GLEW",
        "../libs/GLFW",
		"../libs/FREETYPE",
    }
	
	links {
        "glfw3_mt",
        "glew32s",
        "opengl32",
        "Winmm",
		"freetype",
    }
	
	defines {
        "GLEW_STATIC",
    }
	
	filter "configurations:debug"
        defines {
            "DEBUG"
        }
        symbols "on"
        
    filter "configurations:release"
        defines {
            "NDEBUG"
        }
        optimize "on"
	