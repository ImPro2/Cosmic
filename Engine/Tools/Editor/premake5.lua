project "Editor"
	kind "ConsoleApp"

	flags { "MultiProcessorCompile" }
	
	links
	{
		"Engine",
		"ImGui"
	}

	files
	{
		"Source/**.hpp",
		"Source/**.cpp"
	}

	includedirs
	{
		"../../Core/Engine/Source",
		"%{IncludeDir.GLFW}", -- temporary
		"%{IncludeDir.Glad}", -- temporary
		"%{IncludeDir.GLM}",
		"%{IncludeDir.Optick}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.FontAwesome}"
	}