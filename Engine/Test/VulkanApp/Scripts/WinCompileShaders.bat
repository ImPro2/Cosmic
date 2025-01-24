@echo off

set VERTEX_SHADER_SRC=..\Assets\Shaders\VertexShader.glsl
set FRAGMENT_SHADER_SRC=..\Assets\Shaders\FragmentShader.glsl

set VERTEX_SHADER_SPV=..\Assets\Shaders\VertexShader.spv
set FRAGMENT_SHADER_SPV=..\Assets\Shaders\FragmentShader.spv

set SHADER_SPV=..\Assets\Shaders\Shader.spv

call %VULKAN_SDK%\Bin\glslc.exe -fshader-stage=vertex %VERTEX_SHADER_SRC% -o %VERTEX_SHADER_SPV%
call %VULKAN_SDK%\Bin\glslc.exe -fshader-stage=fragment %FRAGMENT_SHADER_SRC% -o %FRAGMENT_SHADER_SPV%
call %VULKAN_SDK%\Bin\spirv-link.exe %VERTEX_SHADER_SPV% %FRAGMENT_SHADER_SPV% -o %SHADER_SPV%

echo del %VERTEX_SHADER_SPV%
echo del %FRAGMENT_SHADER_SPV%
