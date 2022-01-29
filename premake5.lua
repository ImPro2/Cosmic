configurations
{
	"Debug",
	"Release",
	"Dist"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "Cosmic"
	architecture "x64"
	startproject "Sandbox"
	language "C++"
	cppdialect "C++latest"
	staticruntime "Off"

	targetdir ("%{wks.location}/bin/"     .. outputdir .. "/%{prj.name}")
	objdir    ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "CS_DEBUG"
		runtime "Debug"
		symbols "On"

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


group "Engine"
	group "Engine/Core"
		include "Engine/Core/Engine"
	
	group "Engine/Test"
		include "Engine/Test/Sandbox"
group ""