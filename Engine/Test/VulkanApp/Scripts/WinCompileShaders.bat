@echo off

call %VULKAN_SDK%\Bin\glslc.exe -fshader-stage=vertex ..\Assets\Shaders\VertexShader.glsl -o ..\Assets\Shaders\VertexShader.spv
call %VULKAN_SDK%\Bin\glslc.exe -fshader-stage=fragment ..\Assets\Shaders\FragmentShader.glsl -o ..\Assets\Shaders\FragmentShader.spv
