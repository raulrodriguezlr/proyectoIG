#version 330 core

layout (location = 0) in vec3 inPosition;

uniform vec3 uColor;
uniform mat4 uPVM;

out     vec3 vColor;

void main() {

    vec4 inPositionH = vec4(inPosition,1);

    gl_Position = uPVM * inPositionH;

    vColor = uColor;

}
