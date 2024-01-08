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

VULKAN_SDK = os.getenv("VULKAN_SDK")
IncludeDir = {}
IncludeDir["GLFW"] = "CKR Engine/Vendor/GLFW/include"
IncludeDir["Glad"]	= "CKR Engine/Vendor/Glad/include"
IncludeDir["imgui"]	= "CKR Engine/Vendor/imgui"
IncludeDir["Glm"]	= "CKR Engine/Vendor/Glm"
IncludeDir["stb_image"]	= "CKR Engine/Vendor/stb_image"
IncludeDir["entt"]	= "CKR Engine/Vendor/entt"
IncludeDir["yaml"]	= "CKR Engine/Vendor/yaml/include"
IncludeDir["ImGuizmo"]	= "CKR Engine/Vendor/ImGuizmo"
IncludeDir["VulkanSDK"]	= "CKR Engine/Vendor/VulkanSDK"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/CKR Engine/Vendor/VulkanSDK/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
Library["SPIRV_Tools"] = "%{LibraryDir.VulkanSDK}/SPIRV-Tools.lib"

-- Include this so that it can link to the project
include "CKR Engine/Vendor/GLFW"
include "CKR Engine/Vendor/Glad"
include "CKR Engine/Vendor/imgui"
include "CKR Engine/Vendor/yaml"

project "CKR Engine"
	location "CKR Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

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
		"%{prj.name}/Vendor/Glm/glm/**.inl",

		"%{prj.name}/Vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/Vendor/ImGuizmo/ImGuizmo.cpp"
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
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.VulkanSDK}"
	}

	links{
		"GLFW",
		"Glad",
		"imgui",
		"yaml-cpp",
		"opengl32.lib",
		"dwmapi.lib"
		
	}

	filter "files:CKR Engine/Vendor/ImGuizmo/ImGuizmo.cpp"
		flags { "NoPCH" }

	filter "system:windows"
	systemversion "latest"
	-- define the preprocessor
	defines{
		"GE_BUILD_DLL",
		"GE_PLATFORM_WINDOW",
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE"
	}
	

	filter "configurations:Debug"
	defines "GE_DEBUG"
	runtime "Debug"
	symbols "on"

	links
	{
		"%{Library.ShaderC_Debug}",
		"%{Library.SPIRV_Cross_Debug}",
		"%{Library.SPIRV_Cross_GLSL_Debug}"
	}

	filter "configurations:Release"
	defines "GE_RELEASE"
	runtime "Release"
	optimize "on"

	links
	{
		"%{Library.ShaderC}",
		"%{Library.SPIRV_Cross}",
		"%{Library.SPIRV_Cross_GLSL}"
	}


	filter "configurations:Dist"
	defines "GE_DIST"
	runtime "Release"
	optimize "on"

	links
	{
		"%{Library.ShaderC}",
		"%{Library.SPIRV_Cross}",
		"%{Library.SPIRV_Cross_GLSL}"
	}


	
		
project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

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


project "CKREngine-Editor"
	location "CKREngine-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

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
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
	}

	links{
		"CKR Engine"
	}

	filter "system:windows"
	systemversion "latest"

	defines{
		"GE_PLATFORM_WINDOW",
		--"YAML_CPP_STATIC_DEFINE"
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
