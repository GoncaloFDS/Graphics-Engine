#version 330 core

uniform vec3 LightPosition;
uniform mat4 ViewMatrix;
uniform mat3 NormalMat;
uniform sampler2D tex;
uniform vec3 cameraPos;

in vec3 exPosition;
in vec2 exTexCoord;
in vec3 exNormal;
in vec3 WorldModelLight;
out vec4 FragmentColor;


const vec3 ambient_color = vec3(0.025, 0.025, 0.025);
const vec3 diffuse_color = vec3(0.40, 0.40, 0.40);
const vec3 specular_color = vec3(1.0, 1.0, 1.0);
const float shininess = 128.0 * 0.6;

void main(){
	vec3 E = normalize(cameraPos);	//E.x = ViewMatrix[0][3];
	//E.y = ViewMatrix[1][3];
	//E.z = ViewMatrix[2][3];
	vec3 EyeDirection = normalize(E - exPosition);
	vec3 L = normalize(WorldModelLight - exPosition);
	vec3 H = normalize(L + EyeDirection);
	vec3 N = normalize(exNormal);
	float diffuse_component = max(dot(N, L),0);
	float specular_component = 0.0;
	//if(diffuse_component > 0.0)
		specular_component = pow(max(dot(H, N),0),shininess);
	
	//FragmentColor = vec4(vec3(ambient_color + diffuse_component*diffuse_color + specular_component * specular_color), 1.0);
	FragmentColor = vec4(vec3(ambient_color + diffuse_component*diffuse_color + specular_component * specular_color), 1.0) + texture(tex, exTexCoord);
	//FragmentColor = texture(tex,exTexCoord);
}