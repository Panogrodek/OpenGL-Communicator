project "Client"
	kind "consoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin/obj/%{cfg.buildcfg}"
	
	pchheader "pch.h"
    pchsource "pch/pch.cpp"
	
	files{
		"**.cpp",
		"**.hpp",
		"**.h",
	}
	
	includedirs{
		"include",
		"../include",
		"../include/FREETYPE",
		"../Polonium/include",
		"pch",
	}
	
	links {
		"Polonium"
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