#version 330 core

in vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;

out vec3 exPosition;
out vec2 exTexCoord;
out vec3 exNormal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;
out vec3 WorldModelLight;

void main(void)
{
	exPosition = inPosition;
	exTexCoord = inTexCoord;
	exNormal = inNormal;
	
	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
	exNormal = NormalMatrix * inNormal;
	exPosition = (ModelMatrix*MCPosition).xyz;
	WorldModelLight = vec3(NormalMatrix * LightPosition);
}