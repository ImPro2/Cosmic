#version 450

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

layout(binding = 0) uniform UniformBufferObject
{
	mat4 ViewProjectionMatrix;
	mat4 TransformMatrix;
} ubo;

layout(location = 0) out vec3 vColor;
layout(location = 1) out vec2 vTexCoord;

void main()
{
	gl_Position = ubo.ViewProjectionMatrix * ubo.TransformMatrix * vec4(aPosition, 1.0);

	vColor = aColor;
	vTexCoord = aTexCoord;
}