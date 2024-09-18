project "Engine"
	kind "StaticLib"

	pchheader "cspch.hpp"
	pchsource "Source/cspch.cpp"

	flags { "MultiProcessorCompile" }

	files
	{
		"Source/**.cpp",
		"Source/**.hpp"
	}

	includedirs
	{
		"Source",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.YamlCPP}",
		"%{IncludeDir.Optick}",
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.FontAwesome}"
	}

	libdirs
	{
		"%{wks.location}\\Dependencies\\Optick\\bin\\vs2022\\x64\\Debug"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"Yaml-CPP",
		"stb_image",
		"OptickCore"
	}