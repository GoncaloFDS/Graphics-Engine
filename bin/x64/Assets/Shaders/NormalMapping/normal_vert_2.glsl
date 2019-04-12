#version 330 core

in vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;
in vec4 inTangent;
in vec3 inBiTangent;


out vec3 exPosition;
out vec2 exTexCoord;
out mat3 TBN;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;
uniform vec3 cameraPos;
uniform vec4 plane;


void main(void)
{
	//calculate position and TBN and light position and eye position in tangent space.
	exTexCoord = inTexCoord;
	vec4 MCPosition = vec4(inPosition, 1.0);
	mat4 ModelView = ViewMatrix*ModelMatrix;
	mat4 NormalMatrix = transpose(inverse(ModelView));
	vec3 fragPos = (ModelView * MCPosition).xyz;
	vec3 T = normalize((NormalMatrix * inTangent).xyz);
	vec3 N = normalize((NormalMatrix * vec4(inNormal, 1.0)).xyz);
	vec3 B = normalize((NormalMatrix * vec4(inBiTangent, 1.0)).xyz);
	//T = T  - (dot(T,N)*N);
	//vec3 B = cross(N,T);

	mat3 TBN = transpose(mat3(T,B,N));
	vec3 WorldLightPos = (ViewMatrix * vec4(LightPosition,1.0)).xyz;
	TangentLightPos = TBN * WorldLightPos;
	TangentViewPos = TBN * cameraPos;
	TangentFragPos = TBN * fragPos;


	vec4 clipPosition = ModelMatrix * MCPosition;
	gl_ClipDistance[0] = dot(clipPosition, plane);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;

}