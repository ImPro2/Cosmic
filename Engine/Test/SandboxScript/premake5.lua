configurations
{
	"Debug",
	"Release",
	"Dist"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "SandboxScript"
	architecture "x64"
	language "C++"
	cppdialect "C++latest"
	staticruntime "Off"

	targetdir ("%{wks.location}/bin/"     .. outputdir)
	objdir    ("%{wks.location}/bin-int/" .. outputdir)

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "CS_DEBUG"
		runtime "Debug"
		symbols "On"

		-- links
		-- {
		-- 	"../../../bin/Debug-windows-x86_64/Engine/Engine.lib"
		-- }

	filter "configurations:Release"
		defines "CS_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CS_DIST"
		runtime "Release"
		optimize "On"

	filter 'files:**.ixx'
		compileas 'Module'

include "../../../Dependencies.lua"
include "../../Core/Engine"

project "SandboxScript"
	kind "SharedLib"

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
		"../../Core/Engine/Source",
		"../../../Dependencies/GLM",
		"../../../Dependencies/Optick/src",
		"../../../Dependencies/EnTT/src"
	}