#version 330 core
in vec2 texCoord;
in float shading;
out vec4 FragColor;

uniform sampler2D theTexture;
void main()
{
    FragColor = texture(theTexture, texCoord) * shading;
}