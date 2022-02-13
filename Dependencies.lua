group "Dependencies"
    include "Dependencies/GLFW"
    include "Dependencies/Glad"
    include "Dependencies/Yaml-CPP"
    include "Dependencies/ImGui"
group ""

IncludeDir = {}
IncludeDir["GLFW"]     = "%{wks.location}/Dependencies/GLFW/include"
IncludeDir["Glad"]     = "%{wks.location}/Dependencies/Glad/include"
IncludeDir["GLM"]      = "%{wks.location}/Dependencies/GLM/include"
IncludeDir["YamlCPP"] = "%{wks.location}/Dependencies/Yaml-CPP/include"
IncludeDir["ImGui"]    = "%{wks.location}/Dependencies/ImGui"