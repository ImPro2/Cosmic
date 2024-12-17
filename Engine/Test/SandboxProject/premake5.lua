newoption {
	trigger     = "cosmic-dir",
	description = "Root path of Cosmic",
	value       = "Path"
}

configurations
{
	"Debug",
	"Release",
	"Dist"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
cosmic_dir = _OPTIONS["cosmic-dir"]

IncludeDir = {}
IncludeDir["Engine"]      = "%{cosmic_dir}/Engine/Core/Engine/Source"
IncludeDir["GLFW"]        = "%{cosmic_dir}/Dependencies/GLFW/include"
IncludeDir["Glad"]        = "%{cosmic_dir}/Dependencies/Glad/include"
IncludeDir["GLM"]         = "%{cosmic_dir}/Dependencies/GLM/"
IncludeDir["stb_image"]   = "%{cosmic_dir}/Dependencies/stb_image/include"
IncludeDir["YamlCPP"]     = "%{cosmic_dir}/Dependencies/Yaml-CPP/include"
IncludeDir["ImGui"]       = "%{cosmic_dir}/Dependencies/ImGui"
IncludeDir["ImGuizmo"]    = "%{cosmic_dir}/Dependencies/ImGuizmo"
IncludeDir["Optick"]      = "%{cosmic_dir}/Dependencies/Optick/src"
IncludeDir["EnTT"]        = "%{cosmic_dir}/Dependencies/EnTT/src"
IncludeDir["FontAwesome"] = "%{cosmic_dir}/Dependencies/FontAwesome"

LibDir = {}
LibDir["Engine"]    = "%{cosmic_dir}/bin/%{outputdir}/Engine"
LibDir["ImGui"]     = "%{cosmic_dir}/bin/%{outputdir}/ImGui"
LibDir["GLFW"]      = "%{cosmic_dir}/bin/%{outputdir}/GLFW"
LibDir["Glad"]      = "%{cosmic_dir}/bin/%{outputdir}/Glad"
LibDir["YamlCPP"]   = "%{cosmic_dir}/bin/%{outputdir}/Yaml-CPP"
LibDir["stb_image"] = "%{cosmic_dir}/bin/%{outputdir}/stb_image"

workspace "SandboxProject"
	architecture "x64"
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

    filter { "system:linux" }
        buildoptions { "-Wno-return-type" }

project "SandboxProject"
	kind "SharedLib"
	flags { "MultiProcessorCompile" }
	
	libdirs
	{
		"%{LibDir.Engine}",
		"%{LibDir.ImGui}",
		"%{LibDir.GLFW}",
		"%{LibDir.Glad}",
		"%{LibDir.YamlCPP}",
		"%{LibDir.stb_image}"
	}

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
		"%{IncludeDir.Engine}",
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
