#type vertex
#version 400 core

layout(location = 0) in vec4  aPosition;
layout(location = 1) in vec4  aColor;
layout(location = 2) in vec2  aTexCoord;
layout(location = 3) in float aTexIndex;
layout(location = 4) in float aTilingFactor;

uniform mat4 uViewProjection;

out vec4  vColor;
out vec2  vTexCoord;
out float vTexIndex;
out float vTilingFactor;

void main()
{
    vColor        = aColor;
    vTexCoord     = aTexCoord;
    vTexIndex     = aTexIndex;
    vTilingFactor = aTilingFactor;

    gl_Position = uViewProjection * vec4(aPosition.x, aPosition.y, 0.0f, 1.0f);
}

#type fragment
#version 400 core

layout(location = 0) out vec4 aFragColor;

in vec4  vColor;
in vec2  vTexCoord;
in float vTexIndex;
in float vTilingFactor;

uniform sampler2D uTextures[16];

void main()
{
    aFragColor = texture(uTextures[int(vTexIndex)], vTexCoord * vTilingFactor) * vColor;
}
