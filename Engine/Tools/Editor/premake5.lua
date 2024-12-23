project "Editor"
	kind "ConsoleApp"

	flags { "MultiProcessorCompile" }

	filter "configurations:Debug"
		debugargs { "--project-dir=%{wks.location}/Engine/Test/SandboxProject/SandboxProject.cosmic" }

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
