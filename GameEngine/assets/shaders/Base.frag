#version 460 core

// for color, if no texture is present, assume texture is RGBA{1, 1, 1, 1} and multiply by COLOR_0 attribute of vertex

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in vec4 Color;

uniform sampler2D albedo_texture;
uniform sampler2D emissive_texture;
uniform sampler2D metallic_roughness_texture;
uniform sampler2D normal_texture;
uniform sampler2D occlusion_texture;

uniform int is_selected;    // 1 or 0

void main()
{
    vec3 ambientStrength = vec3(0.4);

    vec3 direction = vec3(0.0, 1.0, 1.0);
    vec3 norm = normalize(Normal);

    float directionalLight = max(dot(norm, direction), 0.0);

    FragColor = texture(albedo_texture, TexCoord) * (vec4(ambientStrength, 1.0) + directionalLight) + (vec4(0.3, 0.0, 0.0, 0.0) * is_selected);

    //FragColor = Color;
}