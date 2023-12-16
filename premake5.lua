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
IncludeDir["Glm"]	= "CKR Engine/Vendor/Glm"
IncludeDir["stb_image"]	= "CKR Engine/Vendor/stb_image"
IncludeDir["entt"]	= "CKR Engine/Vendor/entt"

include "CKR Engine/Vendor/GLFW"
include "CKR Engine/Vendor/Glad"
include "CKR Engine/Vendor/imgui"

project "CKR Engine"
	location "CKR Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	pchheader "gepch.h"
	pchsource "CKR Engine/src/gepch.cpp"

	-- compile these files when regenerate project
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/Vendor/stb_image/**.cpp",
		"%{prj.name}/Vendor/stb_image/**.h",
		"%{prj.name}/Vendor/Glm/glm/**.hpp",
		"%{prj.name}/Vendor/Glm/glm/**.inl"
		--"%{prj.name}/Vendor/entt/**.hpp"
	}


	defines{
		"_CRT_SECURE_NO_WARNINGS"
	}

	--use to include the directory into the project
	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/Vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.Glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
	}

	links{
		"GLFW",
		"Glad",
		"imgui",
		"opengl32.lib",
		"dwmapi.lib"
		
	}

	filter "system:windows"
	systemversion "latest"
	-- define the preprocessor
	defines{
		"GE_BUILD_DLL",
		"GE_PLATFORM_WINDOW",
		"GLFW_INCLUDE_NONE",
	}
	
	--dont need for static library
	-- build dll
--	postbuildcommands{
	--	'{COPY} "%{cfg.targetdir}\\%{prj.name}.dll" "..\\bin\\Debug-windows-x86_64Game\\"'
--	}

	filter "configurations:Debug"
	defines "GE_DEBUG"
	runtime "Debug"
	symbols "on"

	filter "configurations:Release"
	defines "GE_RELEASE"
	runtime "Release"
	optimize "on"

	filter "configurations:Dist"
	defines "GE_DIST"
	runtime "Release"
	optimize "on"

		
project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"CKR Engine/Vendor/spdlog/include",
		"CKR Engine/src",
		"CKR Engine/Vendor",
		"%{IncludeDir.Glm}"
		
	}

	links{
		"CKR Engine"
	}

	filter "system:windows"
	systemversion "latest"

	defines{
		"GE_PLATFORM_WINDOW"
	}

	filter "configurations:Debug"
	defines "GE_DEBUG"
	runtime "Debug"
	symbols "on"

	filter "configurations:Release"
	defines "GE_RELEASE"
	runtime "Release"
	optimize "on"

	filter "configurations:Dist"
	defines "GE_DIST"
	runtime "Release"
	optimize "on"


project "CKREngine-Editor"
	location "CKREngine-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "%{prj.name}")

	
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"CKR Engine/Vendor/spdlog/include",
		"CKR Engine/src",
		"CKR Engine/Vendor",
		"%{IncludeDir.Glm}",
		"%{IncludeDir.entt}"
		
	}

	links{
		"CKR Engine"
	}

	filter "system:windows"
	systemversion "latest"

	defines{
		"GE_PLATFORM_WINDOW"
	}

	filter "configurations:Debug"
	defines "GE_DEBUG"
	runtime "Debug"
	symbols "on"

	filter "configurations:Release"
	defines "GE_RELEASE"
	runtime "Release"
	optimize "on"

	filter "configurations:Dist"
	defines "GE_DIST"
	runtime "Release"
	optimize "on"