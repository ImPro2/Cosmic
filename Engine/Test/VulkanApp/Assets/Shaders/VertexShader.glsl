#version 450

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

layout(binding = 0) uniform UniformBufferObject
{
	mat4 ViewProjectionMatrix;
	mat4 TransformMatrix;
} ubo;

layout(location = 0) out vec3 vColor;

void main()
{
	gl_Position = ubo.ViewProjectionMatrix * ubo.TransformMatrix * vec4(aPosition, 0.0, 1.0);
	vColor = aColor;
}