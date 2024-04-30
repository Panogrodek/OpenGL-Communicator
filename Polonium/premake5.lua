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
		"pch",
	}
	
	libdirs {
        "../libs/GLEW",
        "../libs/GLFW",
    }
	
	links {
        "glfw3_mt",
        "glew32s",
        "opengl32",
        "Winmm",
    }
	
	defines {
        "GLEW_STATIC",
		"STB_IMAGE_IMPLEMENTATION",
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
	