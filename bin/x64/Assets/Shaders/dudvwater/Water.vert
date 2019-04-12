#version 400 core

layout (location = 0) in vec3 inPosition;


out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 fromLightVector;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;
uniform vec3 cameraPos;
uniform vec3 LightPosition;
const float tiling = 6.0;

void main(void) {
	vec4 worldPosition = ModelMatrix * vec4 (inPosition, 1.0);
    clipSpace = ProjectionMatrix * ViewMatrix * worldPosition;
    gl_Position = clipSpace;
    textureCoords = vec2(inPosition.x/2.0 + 0.5, inPosition.z/2.0 + 0.5) * tiling;
    toCameraVector = cameraPos - worldPosition.xyz;
    fromLightVector = worldPosition.xyz - LightPosition;
}