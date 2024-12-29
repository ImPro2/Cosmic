VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"]        = "%{wks.location}/Dependencies/GLFW/include"
IncludeDir["Glad"]        = "%{wks.location}/Dependencies/Glad/include"
IncludeDir["GLM"]         = "%{wks.location}/Dependencies/GLM/"
IncludeDir["stb_image"]   = "%{wks.location}/Dependencies/stb_image/include"
IncludeDir["YamlCPP"]     = "%{wks.location}/Dependencies/Yaml-CPP/include"
IncludeDir["ImGui"]       = "%{wks.location}/Dependencies/ImGui"
IncludeDir["ImGuizmo"]    = "%{wks.location}/Dependencies/ImGuizmo"
IncludeDir["Optick"]      = "%{wks.location}/Dependencies/Optick/src"
IncludeDir["EnTT"]        = "%{wks.location}/Dependencies/EnTT/src"
IncludeDir["FontAwesome"] = "%{wks.location}/Dependencies/FontAwesome"

IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
