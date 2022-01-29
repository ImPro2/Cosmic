project "Sandbox"
	kind "ConsoleApp"
	
	links
	{
		"Engine"
	}

	files
	{
		"Source/**.hpp",
		"Source/**.cpp",
		"Source/**.ixx"
	}

	includedirs
	{
		"../../Core/Engine/Source"
	}