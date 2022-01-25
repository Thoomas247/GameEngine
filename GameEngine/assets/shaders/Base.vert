#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aColor;
layout (location = 4) in ivec4 aJoints;
layout (location = 5) in vec4 aWeights;


uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec4 Color;

void main()
{
    FragPos = vec3(model_mat * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model_mat))) * aNormal;
    TexCoord = aTexCoord;
    Color = aColor;
    
    gl_Position = projection_mat * view_mat * vec4(FragPos, 1.0);
}