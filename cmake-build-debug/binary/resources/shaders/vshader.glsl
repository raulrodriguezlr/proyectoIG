#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexture;

uniform mat4 uN;
uniform mat4 uM;
uniform mat4 uPVM;

out vec3 vnor;
out vec3 vpos;
out vec2 vtex;

void main() {

    vec4 inPositionH = vec4(inPosition,1);
    vec4 inNormalH   = vec4(inNormal  ,0);

    gl_Position = uPVM * inPositionH;

    vnor = (uN*inNormalH).xyz;
    vpos = (uM*inPositionH).xyz;
    vtex = inTexture;

}