workspace "CKR Engine"
	architecture "x64"
	startproject "Game"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "CKR Engine/Vendor/GLFW/include"

include "CKR Engine/Vendor/GLFW"


project "CKR Engine"
	location "CKR Engine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	pchheader "gepch.h"
	pchsource "CKR Engine/src/gepch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/Vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links{
		"GLFW",
		"opengl32.lib",
		"dwmapi.lib"
	}

	filter "system:windows"
	cppdialect "C++20"
	staticruntime "On"
	systemversion "latest"

	defines{
		"GE_BUILD_DLL",
		"GE_PLATFORM_WINDOW"
	}
	
	postbuildcommands{
		'{COPY} "%{cfg.targetdir}\\%{prj.name}.dll" "..\\bin\\Debug-windows-x86_64Game\\"'
	}

	filter "configurations:Debug"
	defines "GA_DEBUG"
	buildoptions "/MD"
	symbols "On"

	filter "configurations:Release"
	defines "GA_RELEASE"
	buildoptions "/MD"
	optimize "On"

	filter "configurations:Dist"
	defines "GA_DIST"
	buildoptions "/MD"
	optimize "On"

		
project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"CKR Engine/Vendor/spdlog/include",
		"CKR Engine/src"
	}

	links{
		"CKR Engine"
	}

	filter "system:windows"
	cppdialect "C++20"
	staticruntime "On"
	systemversion "latest"

	defines{
		"GE_PLATFORM_WINDOW"
	}

	filter "configurations:Debug"
	defines "GA_DEBUG"
	buildoptions "/MD"
	symbols "On"

	filter "configurations:Release"
	defines "GA_RELEASE"
	buildoptions "/MD"
	optimize "On"

	filter "configurations:Dist"
	defines "GA_DIST"
	buildoptions "/MD"
	optimize "On"