#version 330 core
layout (location = 0) in ivec3 pos;
layout (location = 1) in vec2 texCoord_IN;
layout (location = 2) in int aoLvl;

uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;

out vec2 texCoord;
out float shading;

const float aoValues[4] = float[4](0.25, 0.35, 0.75, 1.0);

void main()
{
    gl_Position = projectionViewMatrix * modelMatrix * vec4(pos, 1.0f);
    shading = aoValues[aoLvl];
    texCoord = texCoord_IN;
}