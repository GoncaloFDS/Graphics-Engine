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
uniform vec4 plane;

out vec3 WorldModelLight;

void main(void)
{
	exPosition = inPosition;
	exTexCoord = inTexCoord;
	exNormal = inNormal;
	mat4 normMat = transpose(inverse(ViewMatrix*ModelMatrix));
	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
	exNormal = mat3(normMat) * inNormal;
	exPosition = (ViewMatrix* ModelMatrix*MCPosition).xyz;
	WorldModelLight = vec3(ViewMatrix * vec4(LightPosition,1.0));

	vec4 clipPosition = ModelMatrix * MCPosition;
	gl_ClipDistance[0] = dot(clipPosition, plane);
}