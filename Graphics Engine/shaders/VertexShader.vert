#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexture;
layout (location = 2) in vec3 inNormal;

out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;

void main(void) {
	exPosition = inPosition;
	exTexcoord = inTexture;
	exNormal = inNormal;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition);
}
