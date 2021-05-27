workspace "Eclipse"
	architecture "x64"
	startproject "Eclipse"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Eclipse"
	location "Eclipse"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "%{prj.name}/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
    "%{prj.name}/src/**.hpp"
	}

	includedirs
	{
		"Dep",
		"Eclipse/src",
    "Dep/IMGUI/imgui-master",
    "Dep/RAPIDJSON/rapidjson-master/include/rapidjson",
    "Dep/GLEW/include/GL",
    "Dep/STB/stb-master",
    "Dep/GLM/glm",
    "Dep/GLFW/include/GLFW",
    "Dep/GLM/glm/gtc",
    "Dep/GLM/glm/gtx",
    "Dep/GLEW/lib/Release/x64",
    "Eclipse/src/Graphics/Still_Cleaning",
    "Eclipse/src/Graphics/RendererAPI",
    "Eclipse/src/Graphics/Parser",
    "Eclipse/src/Graphics/ModelFactory/BasicPrimitives",
    "Eclipse/src/Graphics/ModelFactory",
    "Eclipse/src/Graphics/FrameBuffer",
    "Dep/mono/include/mono-2.0"
	}

  libdirs
  {
    "Dep/mono/lib/",
    "Dep/GLFW/lib-vc2019",
    "Dep/GLEW/lib/Release/x64"
  }

  links
  {
    "mono-2.0-sgen.lib",
    "opengl32.lib",
    "glew32.lib",
    "glfw3dll.lib"
  }

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ENGINE_PLATFORM_WINDOWS",
		}


	filter "configurations:Debug"
		defines "ENGINE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "ENGINE_DIST"
		optimize "On"
	