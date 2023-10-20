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
IncludeDir["Glad"]	= "CKR Engine/Vendor/Glad/include"
IncludeDir["imgui"]	= "CKR Engine/Vendor/imgui"

include "CKR Engine/Vendor/GLFW"
include "CKR Engine/Vendor/Glad"
include "CKR Engine/Vendor/imgui"

project "CKR Engine"
	location "CKR Engine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imgui}"

	}

	links{
		"GLFW",
		"Glad",
		"imgui",
		"opengl32.lib",
		"dwmapi.lib"
		
	}

	filter "system:windows"
	cppdialect "C++20"
	systemversion "latest"

	defines{
		"GE_BUILD_DLL",
		"GE_PLATFORM_WINDOW",
		"GLFW_INCLUDE_NONE",
	}
	
	postbuildcommands{
		'{COPY} "%{cfg.targetdir}\\%{prj.name}.dll" "..\\bin\\Debug-windows-x86_64Game\\"'
	}

	filter "configurations:Debug"
	defines "GE_DEBUG"
	runtime "Debug"
	symbols "On"

	filter "configurations:Release"
	defines "GE_RELEASE"
	runtime "Release"
	optimize "On"

	filter "configurations:Dist"
	defines "GE_DIST"
	runtime "Release"
	optimize "On"

		
project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
	systemversion "latest"

	defines{
		"GE_PLATFORM_WINDOW"
	}

	filter "configurations:Debug"
	defines "GE_DEBUG"
	runtime "Debug"
	symbols "On"

	filter "configurations:Release"
	defines "GE_RELEASE"
	runtime "Release"
	optimize "On"

	filter "configurations:Dist"
	defines "GE_DIST"
	runtime "Release"
	optimize "On"