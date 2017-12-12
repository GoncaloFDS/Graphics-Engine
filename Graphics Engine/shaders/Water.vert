#version 400 core

layout (location = 0) in vec3 inPosition;


out vec4 clipSpace;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;

void main(void) {
    clipSpace = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4 (inPosition, 1.0);
    gl_Position = clipSpace;
}