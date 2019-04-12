#version 330 core

in vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;
in vec4 inTangent;
in vec3 inBiTangent;


out vec3 exPosition;
out vec2 exTexCoord;
out vec3 exNormal;
out mat3 TBN;


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
	mat4 modelView = ViewMatrix * ModelMatrix;
	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
	exNormal = mat3(normMat) * inNormal;
	vec3 T = normalize((normMat * inTangent).xyz);
	vec3 B = normalize((normMat * vec4(inBiTangent,1.0)).xyz);
	vec3 N = normalize((normMat * vec4(inNormal, 1.0)).xyz);
	TBN = transpose(mat3(T,B,N));

	exPosition = (ViewMatrix* ModelMatrix*MCPosition).xyz;
	WorldModelLight = vec3(ViewMatrix * vec4(LightPosition,1.0));

	vec4 clipPosition = ModelMatrix * MCPosition;
	gl_ClipDistance[0] = dot(clipPosition, plane);
}