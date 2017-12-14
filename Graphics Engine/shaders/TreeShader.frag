#version 330 core

uniform vec3 LightPosition;
uniform mat4 ViewMatrix;
uniform mat3 NormalMat;
uniform sampler2D tex;

in vec3 exPosition;
in vec2 exTexCoord;
in vec3 exNormal;

out vec4 FragmentColor;

const vec3 ambient_color = vec3(0.545, 0.270, 0.074);
const vec3 diffuse_color = vec3(0.745, 0.470, 0.274);
const vec3 specular_color = vec3(0.633, 0.727811, 0.633);
const float shininess = 128.0 * 0.6;

void main(){
	vec3 E;
	E.x = ViewMatrix[0][3];
	E.y = ViewMatrix[1][3];
	E.z = ViewMatrix[2][3];
	vec3 L = normalize(LightPosition - exPosition);
	vec3 H = normalize(L + E);
	vec3 N = normalize(exNormal);
	float diffuse_component = max(dot(N, L),0);
	float specular_component = pow(max(dot(H, N),0),shininess);
	//FragmentColor = vec4(vec3(ambient_color + diffuse_component*diffuse_color + specular_component * specular_color), 1.0);
	//FragmentColor = vec4(vec3(ambient_color + diffuse_component*diffuse_color + specular_component * specular_color), 1.0) + texture(tex, exTexCoord);
	FragmentColor = texture(tex,exTexCoord);
}