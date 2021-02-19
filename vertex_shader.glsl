#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec2 TexCoords;
layout(location = 1) out vec3 WorldPos;
layout(location = 2) out vec3 Normal;
layout(location = 3) out vec3 cameraPos;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    TexCoords = inTexCoord;
    WorldPos = vec3((ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0)).xyz);
    Normal = inNormal;
    cameraPos = vec3((ubo.view * ubo.model * vec4(inPosition, 1.0)).xyz);
}