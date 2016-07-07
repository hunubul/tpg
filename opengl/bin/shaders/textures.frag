#version 430 core
in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    outColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
}