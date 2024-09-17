project "Engine"
	kind "StaticLib"

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

	pchheader "cspch.hpp"
	pchsource "cspch.cpp"

	libdirs
	{
		"%{wks.location}/bin/Debug-windows-x86_64/Optick"
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