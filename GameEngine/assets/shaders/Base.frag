#version 460 core

// for color, if no textrue is present, assume texture is RGBA{1, 1, 1, 1} and multiply by COLOR_0 attribute of vertex

out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;

uniform sampler2D albedo_texture;
uniform sampler2D emissive_texture;
uniform sampler2D metallic_roughness_texture;
uniform sampler2D normal_texture;
uniform sampler2D occlusion_texture;


void main()
{
    FragColor = texture(albedo_texture, TexCoord);
    //FragColor = Color;
}