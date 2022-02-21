#start vertex
#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 aColor;
layout(location = 4) in ivec4 aJoints;
layout(location = 5) in vec4 aWeights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec4 Color;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

void main()
{
    FragPos = vec3(model_mat * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model_mat))) * aNormal;
    TexCoord = aTexCoord;
    Color = aColor;

    gl_Position = projection_mat * view_mat * vec4(FragPos, 1.0);
}

#end vertex



#start fragment
#version 460 core

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in vec4 Color;

uniform vec4 albedo_fac;
uniform vec3 emissive_fac;
uniform float metallic_fac;
uniform float roughness_fac;

uniform sampler2D albedo_tex;
uniform sampler2D emissive_tex;
uniform sampler2D metallic_roughness_tex;
uniform sampler2D normal_tex;
uniform sampler2D occlusion_tex;

void main()
{
    FragColor = texture(albedo_tex, TexCoord);

    if (FragColor.a < 0.5)
        discard;

    vec3 ambientStrength = vec3(0.4);

    vec3 direction = vec3(0.0, 1.0, 1.0);
    vec3 norm = normalize(Normal);

    float directionalLight = max(dot(norm, direction), 0.0);

    FragColor = FragColor * (vec4(ambientStrength, 1.0) + directionalLight);
}

#end fragment