#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emissive;
	float shininess;
};

uniform vec3 LightPosition;
uniform mat4 ViewMatrix;
uniform mat3 NormalMat;
uniform sampler2D tex;
uniform vec3 cameraPos;
uniform Material mat;

in vec3 exPosition;
in vec2 exTexCoord;
in vec3 exNormal;
in vec3 WorldModelLight;
out vec4 FragmentColor;


const vec3 ambient_light = vec3(0.025, 0.025, 0.025);
const vec3 diffuse_light = vec3(0.40, 0.40, 0.40);
const vec3 specular_light = vec3(1.0, 1.0, 1.0);
const float shininess_factor = 128.0;

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
	if(diffuse_component > 0.0)
		specular_component = pow(max(dot(H, N),0),mat.shininess*shininess_factor);
		//specular_component = pow(max(dot(H,N), 0), shininess_factor);

	vec3 final_ambient_color = ambient_light * mat.ambient;
	vec3 final_diffuse_color = diffuse_light * mat.diffuse;
	vec3 final_specular_color = specular_light * mat.specular; 
	
	//FragmentColor = vec4(vec3(ambient_light + diffuse_component * diffuse_light + specular_component * specular_light),1.0);

	FragmentColor = vec4(vec3(final_ambient_color + diffuse_component*final_diffuse_color + specular_component * final_specular_color), 1.0);
	//FragmentColor = vec4(vec3(final_ambient_color + diffuse_component*final_diffuse_color + specular_component * final_specular_color), 1.0) + texture(tex, exTexCoord);
	//FragmentColor = texture(tex,exTexCoord);
}