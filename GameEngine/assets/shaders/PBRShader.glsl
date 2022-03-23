#start vertex
#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform ModelViewProjection {
    mat4 model;
    mat4 view;
    mat4 proj;
} mvp;

void main() {
    gl_Position = mvp.proj * mvp.view * mvp.model * vec4(inPosition, 1.0);
    //gl_Position = vec4(inPosition, 1.0);
    fragColor = inColor;
}

#end vertex


#start fragment
#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 fragColor;

void main() 
{
    outColor = fragColor;
}

#end fragment