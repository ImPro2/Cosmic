project "Engine"
	kind "StaticLib"

	files
	{
		"Source/**.cpp",
		"Source/**.ixx",
		"Source/**.hpp"
	}
	
	-- group "Dependencies/GLM"
	-- 	files
	-- 	{
	-- 		"%{wks.location}/Dependencies/GLM/glm/**.hpp",
	--		"%{wks.location}/Dependencies/GLM/glm/**.inl"
	-- 	}
	-- group ""

	includedirs
	{
		"Source",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.YamlCPP}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"Yaml-CPP"
	}