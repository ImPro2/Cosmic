project "stb_image"
	kind "StaticLib"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/stb_image.h",
		"include/stb_image.cpp"
	}

	includedirs
	{
		"include"
	}
