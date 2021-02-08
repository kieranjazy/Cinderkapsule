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
layout(location = 4) in vec3 inCameraPos; //remove this and use model matrix transformation multiplication instead
layout(location = 5) in vec3 inLightPos1; //make these positions uniforms
layout(location = 6) in vec3 inLightPos2;
layout(location = 7) in vec3 inLightColour1;
layout(location = 8) in vec3 inLightColour2;

layout(location = 0) out vec2 TexCoords;
layout(location = 1) out vec3 WorldPos;
layout(location = 2) out vec3 Normal;
layout(location = 3) out vec3 cameraPos;
layout(location = 4) out vec3 outLightPos1;
layout(location = 5) out vec3 outLightPos2;
layout(location = 6) out vec3 outLightColour1;
layout(location = 7) out vec3 outLightColour2;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    TexCoords = inTexCoord;
    WorldPos = inPosition;
    Normal = inNormal;
    cameraPos = inCameraPos;

    outLightPos1 = inLightPos1;
    outLightPos2 = inLightPos2;
    outLightColour1 = inLightColour1;
    outLightColour2 = inLightColour2;

}