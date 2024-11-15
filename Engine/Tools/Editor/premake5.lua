project "Editor"
	kind "ConsoleApp"

	flags { "MultiProcessorCompile" }

	links
	{
	    "Engine",
        "ImGui",
        "GLFW",
        "Glad",
        "Yaml-CPP",
        "stb_image"
	}

	files
	{
		"Source/**.hpp",
		"Source/**.cpp"
	}

	includedirs
	{
        "Source",
		"../../Core/Engine/Source",
		"%{IncludeDir.GLFW}", -- temporary
		"%{IncludeDir.Glad}", -- temporary
		"%{IncludeDir.GLM}",
		"%{IncludeDir.Optick}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.FontAwesome}"
	}
