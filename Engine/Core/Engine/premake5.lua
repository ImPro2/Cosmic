project "Engine"
	kind "StaticLib"

	pchheader "cspch.hpp"
	pchsource "Source/cspch.cpp"

	files
	{
		"Source/**.cpp",
		"Source/**.ixx",
		"Source/**.hpp"
	}

	includedirs
	{
		"Source"
	}