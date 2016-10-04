#version 430 core
in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D ourTexture1;

void main()
{
    outColor = texture(ourTexture1, TexCoord);
}