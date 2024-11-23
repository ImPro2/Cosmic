#type vertex
#version 450 core

layout(location = 0) in vec4  aPosition;
layout(location = 1) in vec4  aColor;
layout(location = 2) in vec2  aTexCoord;
layout(location = 3) in float aTexIndex;
layout(location = 4) in float aTilingFactor;
layout(location = 5) in int aEntityID;

uniform mat4 uViewProjection;

layout(location = 0) out vec4     vColor;
layout(location = 1) out vec2     vTexCoord;
layout(location = 2) out float    vTexIndex;
layout(location = 3) out float    vTilingFactor;
layout(location = 4) out flat int vEntityID;

void main()
{
    vColor        = aColor;
    vTexCoord     = aTexCoord;
    vTexIndex     = aTexIndex;
    vTilingFactor = aTilingFactor;
    vEntityID     = aEntityID;

    gl_Position = uViewProjection * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0f);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 oFragColor;
layout(location = 1) out int  oEntityID;

layout(location = 0) in vec4  vColor;
layout(location = 1) in vec2  vTexCoord;
layout(location = 2) in float vTexIndex;
layout(location = 3) in float vTilingFactor;
layout(location = 4) in flat int vEntityID;

uniform sampler2D uTextures[16];

void main()
{
    oFragColor = texture(uTextures[int(vTexIndex)], vTexCoord * vTilingFactor) * vColor;
    oEntityID  = vEntityID;
}

