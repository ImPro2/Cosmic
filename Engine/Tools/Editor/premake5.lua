project "Editor"
	kind "ConsoleApp"
	
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

	pchheader "cspch.hpp"
	pchsource "cspch.cpp"

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