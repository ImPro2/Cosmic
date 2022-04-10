#type vertex
#version 400 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 uViewProjection;
uniform mat4 uTransform;
uniform vec3 uColor;

out vec2 vTexCoord;
out vec4 vColor;

void main()
{
    vTexCoord = aTexCoord;
    vColor    = vec4(uColor, 1.0);

    gl_Position = uViewProjection * uTransform * vec4(aPosition, 0.0, 1.0);
}

#type fragment
#version 400 core

layout(location = 0) out vec4 aFragColor;

in vec2 vTexCoord;
in vec4 vColor;

uniform sampler2D uTexture;

void main()
{
    aFragColor = texture(uTexture, vTexCoord);
    //aFragColor = vColor;
}