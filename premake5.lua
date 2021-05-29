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
    "%{prj.name}/src/**.hpp",
    "Dep/IMGUI/imgui-master/imgui_widgets.cpp",
    "Dep/IMGUI/imgui-master/imgui_draw.cpp",
    "Dep/IMGUI/imgui-master/imgui_demo.cpp",
    "Dep/IMGUI/imgui-master/imgui_internal.h",
    "Dep/IMGUI/imgui-master/imgui.cpp",
    "Dep/IMGUI/imgui-master/imgui.h",
    "Dep/IMGUI/imgui-master/examples/imgui_impl_opengl3.cpp",
    "Dep/IMGUI/imgui-master/examples/imgui_impl_opengl3.h",
    "Dep/IMGUI/imgui-master/examples/imgui_impl_glfw.cpp",
    "Dep/IMGUI/imgui-master/examples/imgui_impl_glfw.h"
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
	}

  libdirs
  {
    "Dep/GLFW/lib-vc2019",
    "Dep/GLEW/lib/Release/x64"
  }

  links
  {
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
	