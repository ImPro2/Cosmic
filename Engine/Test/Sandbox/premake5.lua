project "Sandbox"
	kind "ConsoleApp"
	
	links
	{
		"Engine",
		"ImGui"
	}

	files
	{
		"Source/**.hpp",
		"Source/**.cpp",
		"Source/**.ixx"
	}

	includedirs
	{
		"../../Core/Engine/Source",
		"%{IncludeDir.GLFW}", -- temporary
		"%{IncludeDir.Glad}", -- temporary
		"%{IncludeDir.GLM}",
		"%{IncludeDir.Optick}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.EnTT}"
	}