project "VulkanApp"
	kind "ConsoleApp"

	flags { "MultiProcessorCompile" }
	
	links
	{
	    "Engine",
        "ImGui",
        "GLFW",
        "Glad",
        "Yaml-CPP",
        "stb_image",
		"SPIRV-Reflect",
		"%{Library.Vulkan}",
		"%{Library.shaderc}",
		"%{Library.SPIRVTools}"
	}

	--for _, lib in pairs(Library.SPIRV) do
		--links(lib)
	--end

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
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.SPIRVReflect}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Optick}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.FontAwesome}",
		"%{IncludeDir.YamlCPP}"
	}