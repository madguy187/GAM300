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
	warnings "Extra"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "%{prj.name}/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp",
	}

	includedirs
	{
		"Dep",
		"Eclipse/src",
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
		"Eclipse/src/ImGui/Vendor/ImGuizmo",
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
		"Dep/PhysX/include/vehicle",
		"Dep/mono/include/mono-2.0",
		"Dep/ASSIMP/include/assimp",
		"Dep/ASSIMP",
		"Dep/Fmod/core/inc",
		"Dep/Tracy",
		"Eclipse/src/Editor/Windows/NodeEditor/ImNodes"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ENGINE_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS",
			"TRACY_ENABLE"
		}

	filter "configurations:Debug"
		defines "ENGINE_DEBUG"
		symbols "On"
    editAndContinue "Off"

	libdirs
  {
    "Dep/GLFW/lib-vc2019",
    "Dep/GLEW/lib/Release/x64",
	  "Dep/PhysX/Debug",
    "Dep/mono/lib/",
	  "Dep/ASSIMP/",
	  "Dep/Fmod/core/lib/x64",
	  "Dep/Tracy"
  }
  
  disablewarnings 
  { 
    "4302",
    "4311",	
    "4505"	
  }

  links
  {
	  "mono-2.0-sgen.lib",
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
	  "SnippetUtils_static_64.lib",
	  "assimp-vc142-mtd",
	  "fmodL_vc.lib",
	  "fmodL.lib",
	  "fmodstudioL64_vc.lib",
	  "fmodstudioL_vc.lib",
	  "TracyProfiler.lib"
  }

  postbuildcommands
  {
	  "{COPY} ../Dep/PhysX/Debug/glut32.dll %{cfg.targetdir}",
    "{COPY} ../Dep/PhysX/Debug/PhysX_64.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/PhysX/Debug/PhysXCommon_64.dll %{cfg.targetdir}",
    "{COPY} ../Dep/PhysX/Debug/PhysXCooking_64.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/PhysX/Debug/PhysXDevice64.dll %{cfg.targetdir}",
    "{COPY} ../Dep/PhysX/Debug/PhysXFoundation_64.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/PhysX/Debug/PhysXGpu_64.dll %{cfg.targetdir}",
    "{COPY} ../Dep/mono/bin/mono-2.0-sgen.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/ASSIMP/assimp-vc142-mtd.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/Fmod/core/lib/x64/fmodL.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/Fmod/core/lib/x64/fmodL64.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/Fmod/core/lib/x64/fmodstudioL64.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/Fmod/core/lib/x64/fmodstudioL.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/Tracy/TracyProfiler.dll %{cfg.targetdir}",
    "{COPY} ../Dep/GLEW/bin/Release/x64/glew32.dll %{cfg.targetdir}",
    "{COPY} ../Dep/GLFW/lib-mingw-w64/glfw3.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/ASSIMP/assimp-vc142-mtd.dll ../Eclipse",
	  "{COPY} ../Dep/ASSIMP/assimp-vc142-mtd.lib ../Eclipse"
  }

	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		optimize "On"
		
	libdirs
  {
    "Dep/GLFW/lib-vc2019",
    "Dep/GLEW/lib/Release/x64",
	  "Dep/PhysX/Release",
    "Dep/mono/lib/",
	  "Dep/ASSIMP/",
	  "Dep/Fmod/core/lib/x64",
	  "Dep/Tracy"
  }
  
  disablewarnings 
  { 
    "4302",
    "4311",	
    "4505"	
  }

  links
  {
	  "mono-2.0-sgen.lib",
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
	  "SnippetUtils_static_64.lib",
	  "assimp-vc142-mtd",
	  "fmod_vc.lib",
    "fmodstudio64_vc.lib",
	  "TracyProfiler.lib"
  }

  postbuildcommands
  {
    "{COPY} ../Dep/PhysX/Release/glut32.dll %{cfg.targetdir}",
    "{COPY} ../Dep/PhysX/Release/PhysX_64.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/PhysX/Release/PhysXCommon_64.dll %{cfg.targetdir}",
    "{COPY} ../Dep/PhysX/Release/PhysXCooking_64.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/PhysX/Release/PhysXDevice64.dll %{cfg.targetdir}",
    "{COPY} ../Dep/PhysX/Release/PhysXFoundation_64.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/PhysX/Release/PhysXGpu_64.dll %{cfg.targetdir}",
    "{COPY} ../Dep/mono/bin/mono-2.0-sgen.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/ASSIMP/assimp-vc142-mtd.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/Fmod/core/lib/x64/fmod.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/Fmod/core/lib/x64/fmod64.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/Fmod/core/lib/x64/fmodstudio64.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/Tracy/TracyProfiler.dll %{cfg.targetdir}",
    "{COPY} ../Dep/GLEW/bin/Release/x64/glew32.dll %{cfg.targetdir}",
    "{COPY} ../Dep/GLFW/lib-mingw-w64/glfw3.dll %{cfg.targetdir}",
	  "{COPY} ../Dep/ASSIMP/assimp-vc142-mtd.dll ../Eclipse",
	  "{COPY} ../Dep/ASSIMP/assimp-vc142-mtd.lib ../Eclipse"
  }

	filter "configurations:Dist"
		defines "ENGINE_DIST"
		optimize "On"
	
  outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  
project "Compiler"
  location "Compiler"
  kind "WindowedApp"
  language "C++"
  cppdialect "C++17"
  warnings "Extra"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
  
  pchheader "pch.h"
  pchsource "%{prj.name}/Source/pch.cpp"

  files
  {
    "%{prj.name}/**.h",
    "%{prj.name}/**.cpp",
	  "%{prj.name}/**.hpp"
  }

  includedirs
  {
	  "%{prj.name}/Compiler/Geometry",
	  "%{prj.name}/Compiler/ICompiler",
	  "%{prj.name}/Compiler/Prefabs",
	  "%{prj.name}/Compiler/Texture",
	  "%{prj.name}/Compiler/Texture",
	  "%{prj.name}/Compiler",
	  "Dep/GLM/glm",
	  "Dep/",
    "%{prj.name}/Assimp",
	  "%{prj.name}/Source"
  }
  

  filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    libdirs
    {
	    "Dep/ASSIMP/"
    }

    links
    {
      "assimp-vc142-mtd"
    }

  filter "configurations:Debug"
    defines "ENGINE_DEBUG"
    symbols "On"
    editAndContinue "Off"

  filter "configurations:Release"
    defines "ENGINE_RELEASE"
    optimize "On"
    