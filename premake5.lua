workspace "GGEngine"

	language "C++"

	architecture "x64"

	cppdialect "C++17"

	configurations { "Debug", "Release" }


	filter {"system:windows"}
		links { "Shlwapi" }
		systemversion "latest"

	filter {"system:linux"}
		links { "pthread" }
		toolset "clang"
		buildoptions{"-fPIC -Wno-dangling-else"}


	filter "configurations:Debug"
		defines "EPP_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines {"EPP_RELEASE"}
		optimize "On"
	filter{}
	
	includedirs
	{
		"./include/",
		"./external/include/"
	}

	libdirs
	{
		"./external/lib/"
	}

outDir = "%{cfg.system}_%{cfg.architecture}/%{cfg.buildcfg}/"

	targetdir ("bin/" .. outDir .. "%{prj.name}")

	objdir ("bin_inter/" .. outDir .. "%{prj.name}")


project "GGEngine"

	kind "ConsoleApp"

	location "./"

	files
	{
		"%{prj.location}/src/%{prj.name}/**",
		"%{prj.location}/external/src/%{prj.name}/**",
	}

	links { "sfml-graphics", "sfml-window", "sfml-system"}