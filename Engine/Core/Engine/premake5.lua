project "Engine"
	kind "StaticLib"

	files
	{
		"Source/**.cpp",
		"Source/**.ixx",
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
		"%{IncludeDir.EnTT}"
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