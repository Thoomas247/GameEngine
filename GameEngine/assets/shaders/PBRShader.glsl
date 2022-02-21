#start vertex
#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 aColor;

layout(std140, binding = 0) uniform Camera
{
    mat4 view_mat;
    mat4 projection_mat;
};

struct VertexOutput
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 Color;
};

layout(location = 0) out VertexOutput Output;

void main()
{
    Output.FragPos = vec3(model_mat * vec4(aPos, 1.0));
    Output.Normal = mat3(transpose(inverse(model_mat))) * aNormal;
    Output.TexCoord = aTexCoord;
    Output.Color = aColor;

    gl_Position = projection_mat * view_mat * vec4(Output.FragPos, 1.0);
}

#end vertex


#start fragment
#version 460 core

struct VertexOutput
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 Color;
};

layout(location = 0) in VertexOutput Input;

layout(binding = 0) uniform sampler2D albedo_t;
layout(binding = 1) uniform sampler2D emissive_t;
layout(binding = 2) uniform sampler2D metallic_roughness_t;
layout(binding = 3) uniform sampler2D normal_t;
layout(binding = 4) uniform sampler2D occlusion_t;

layout(location = 0) out vec4 OutColor;

void main()
{
    vec4 fragColor = texture(albedo_t, Input.TexCoord);

    if (fragColor.a < 0.5)
        discard;

    vec3 ambientStrength = vec3(0.4);
    vec3 direction = vec3(0.0, 1.0, 1.0);
    vec3 norm = normalize(Input.Normal);
    float directionalLight = max(dot(norm, direction), 0.0);

    OutColor = fragColor * (vec4(ambientStrength, 1.0) + directionalLight);
}


#end fragment