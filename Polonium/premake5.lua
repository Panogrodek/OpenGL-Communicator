project "Polonium"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir "bin/%{cfg.buildcfg}"
    objdir    "bin/obj/%{cfg.buildcfg}"
	
	files{
		"**.cpp",
		"**.hpp",
		"**.h",
	}
	
	includedirs{
		"include",
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
	