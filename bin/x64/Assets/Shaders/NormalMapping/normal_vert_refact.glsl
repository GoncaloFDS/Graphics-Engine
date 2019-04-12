#version 330 core

struct TangentSpace{
	vec3 LightPos;
	vec3 ViewPos;
	vec3 FragPos;
};


layout(location=0) in vec3 inPosition;
layout(location=1) in vec2 inTexCoord;
layout(location=2) in vec3 inNormal;
layout(location=3) in vec4 inTangent;
layout(location=4) in vec3 inBiTangent;


out vec3 exPosition;
out vec2 exTexCoord;
out mat3 TBN;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out TangentSpace tangentSpace;


uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;
uniform vec3 cameraPos;
uniform vec4 plane;


mat3 CalculateTBN(in vec3 inNormal, in vec4 inTangent, in vec3 inBiTangent, in mat4 NormalMatrix){
	vec3 T = normalize((NormalMatrix * inTangent).xyz);
	vec3 B = normalize((NormalMatrix * vec4(inBiTangent,1.0)).xyz);
	vec3 N = normalize((NormalMatrix * vec4(inNormal,1.0)).xyz);
	return mat3(T,B,N);
}

vec3 CalculateLightInWorld(in vec3 LightPosition, in mat4 ViewMatrix){
	return (ViewMatrix * vec4(LightPosition, 1.0)).xyz;
}


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

	mat3 TBN = transpose(CalculateTBN(inNormal, inTangent, inBiTangent, NormalMatrix));
	vec3 WorldLightPos = CalculateLightInWorld(LightPosition, ViewMatrix);
	
	tangentSpace.LightPos = TBN * WorldLightPos;
	tangentSpace.ViewPos = TBN * cameraPos;
	tangentSpace.FragPos = TBN * fragPos;




	vec4 clipPosition = ModelMatrix * MCPosition;
	gl_ClipDistance[0] = dot(clipPosition, plane);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;

}
