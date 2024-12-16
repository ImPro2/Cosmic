project "SandboxScript"
	kind "SharedLib"

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
        "%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.FontAwesome}",
		"%{IncludeDir.YamlCPP}"
	}
