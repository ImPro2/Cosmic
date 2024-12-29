project "Engine"
	kind "StaticLib"

	pchheader "cspch.hpp"
	pchsource "Source/cspch.cpp"

	flags { "MultiProcessorCompile" }

	files
	{
		"Source/**.cpp",
		"Source/**.hpp",
	}

	includedirs
	{
		"Source",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.YamlCPP}",
		"%{IncludeDir.Optick}",
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.FontAwesome}"
	}

	--libdirs
	--{
	--	"%{wks.location}\\Dependencies\\Optick\\bin\\vs2022\\x64\\Debug"
	--}

	links
	{
		"GLFW",
		"Glad",
		"%{Library.Vulkan}",
		"ImGui",
		"Yaml-CPP",
		"stb_image",
		--"OptickCore"
	}

	filter "system:windows"
		links { "shlwapi.lib" }
