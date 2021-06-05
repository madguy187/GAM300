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
	"Dep/spdlog/include",
    "Eclipse/src/Graphics/Still_Cleaning",
    "Eclipse/src/Graphics/RendererAPI",
    "Eclipse/src/Graphics/Parser",
    "Eclipse/src/Graphics/ModelFactory/BasicPrimitives",
    "Eclipse/src/Graphics/ModelFactory",
    "Eclipse/src/Graphics/FrameBuffer",
	"Eclipse/src/Graphics/OpenGL",
	"Eclipse/src/ImGui/Vendor",
	"Dep/PhysX/include",
	"Dep/PhysX/include/characterkinematic",
	"Dep/PhysX/include/collision",
	"Dep/PhysX/include/common",
	"Dep/PhysX/include/cooking",
	"Dep/PhysX/include/cudamanager",
	"Dep/PhysX/include/extensions",
	"Dep/PhysX/include/filebuf",
	"Dep/PhysX/include/foundation",
	"Dep/PhysX/include/geometry",
	"Dep/PhysX/include/geomutils",
	"Dep/PhysX/include/gpu",
	"Dep/PhysX/include/pvd",
	"Dep/PhysX/include/solver",
	"Dep/PhysX/include/task",
	"Dep/PhysX/include/vehicle"
	}

  libdirs
  {
    "Dep/GLFW/lib-vc2019",
    "Dep/GLEW/lib/Release/x64",
	"Dep/PhysX/Release"
  }

  links
  {
    "opengl32.lib",
    "glew32.lib",
    "glfw3dll.lib",
	"LowLevel_static_64.lib",
	"LowLevelAABB_static_64.lib",
	"LowLevelDynamics_static_64.lib",
	"PhysX_64.lib",
	"PhysXCharacterKinematic_static_64.lib",
	"PhysXCommon_64.lib",
	"PhysXCooking_64.lib",
	"PhysXExtensions_static_64.lib",
	"PhysXFoundation_64.lib",
	"PhysXPvdSDK_static_64.lib",
	"PhysXTask_static_64.lib",
	"PhysXVehicle_static_64.lib",
	"SampleBase_static_64.lib",
	"SampleFramework_static_64.lib",
	"SamplesToolkit_static_64.lib",
	"SceneQuery_static_64.lib",
	"SimulationController_static_64.lib",
	"SnippetRender_static_64.lib",
	"SnippetUtils_static_64.lib"
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
	