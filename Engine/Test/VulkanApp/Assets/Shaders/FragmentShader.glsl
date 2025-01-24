#version 450

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTexCoord;

layout(set = 0, binding = 1) uniform sampler2D uTextureSampler;

layout(location = 0) out vec4 oColor;

void main()
{
	oColor = texture(uTextureSampler, vTexCoord * 2.0);
}