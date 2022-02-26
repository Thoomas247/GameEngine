#start vertex
#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 aColor;

layout(std140, binding = 0) uniform Camera
{
    mat4 view_mat;
    mat4 projection_mat;
};

layout(std140, binding = 1) uniform Lights
{
    mat4 light;
    mat4 example;
};

layout(std140, binding = 2) uniform Model
{
    mat4 model_mat;
};

layout(location = 0) out VertexOutput
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 Color;
};

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
#version 450 core
#extension GL_ARB_bindless_texture : require

layout(location = 0) in VertexOutput
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 Color;
};

layout(std140, binding = 2) uniform RenderingInfo {
    sampler2D  texAlbedo;
    sampler2D  texNormal;
    float      bumpIntensity;
};

layout(location = 0) out vec4 OutColor;

void main()
{
    vec4 fragColor = texture(texAlbedo, Input.TexCoord);

    if (fragColor.a < 0.5)
        discard;

    vec3 ambientStrength = vec3(0.4);
    vec3 direction = vec3(0.0, 1.0, 1.0);
    vec3 norm = normalize(Input.Normal);
    float directionalLight = max(dot(norm, direction), 0.0);

    OutColor = fragColor * (vec4(ambientStrength, 1.0) + directionalLight);
}


#end fragment