#version 460 core

// for color, if no textrue is present, assume texture is RGBA{1, 1, 1, 1} and multiply by COLOR_0 attribute of vertex

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D base_texture;

void main()
{
    FragColor = texture(base_texture, TexCoord);
}