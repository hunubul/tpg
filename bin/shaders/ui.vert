#version 430 core
in vec2 position;
in vec2 texcoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position, 0.0f, 1.0f);
    TexCoord = vec2(texcoord.x, 1.0 - texcoord.y);
}