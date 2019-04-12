#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material mat;

uniform vec3 LightPosition;
uniform mat4 ViewMatrix;
uniform mat3 NormalMat;
uniform sampler2D tex;
uniform sampler2D normalMap;


in vec3 exPosition;
in vec2 exTexCoord;

in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;



out vec4 FragmentColor;



const vec3 ambient_light = vec3(0.025, 0.025, 0.025);
const vec3 diffuse_light = vec3(0.40, 0.40, 0.40);
const vec3 specular_light = vec3(0.80, 0.80, 0.80);
const float shininess_factor = 128.0;
in vec3 WorldModelLight;

void main(){
	

	vec3 normal = texture(normalMap, exTexCoord).rgb;

	normal = normalize(normal * 2.0 - 1.0);

	vec3 N = normal;
	vec3 L = normalize((TangentLightPos - TangentFragPos));
	vec3 E = normalize((TangentViewPos - TangentFragPos));
	vec3 H = normalize(L + E);

	float diffuse_component = max(dot(L, N),0);
	float specular_component = 0.0;
	if(diffuse_component > 0.0)
		specular_component = pow(max(dot(N, H),0),shininess_factor * mat.shininess);
		//specular_component = pow(max(dot(H,N),0),shininess_factor);

	vec3 final_ambient_color = ambient_light * mat.ambient;
	vec3 final_diffuse_color = diffuse_light * mat.diffuse;
	vec3 final_specular_color = specular_light * mat.specular; 
	//vec3 final_ambient_color = ambient_light;
	//vec3 final_diffuse_color = diffuse_light;
	//vec3 final_specular_color = specular_light;

	//FragmentColor = vec4(vec3(final_ambient_color + diffuse_component*final_diffuse_color + specular_component*final_specular_color), 1.0);
	FragmentColor = vec4(vec3(final_ambient_color + diffuse_component*final_diffuse_color + specular_component*final_specular_color), 1.0) + texture(tex, exTexCoord);
	
	//FragmentColor = vec4(diffuse_component * final_diffuse_color, 1.0); // only diffuse component
	//FragmentColor = vec4(specular_component * final_specular_color, 1.0); //only specular component
	
	//FragmentColor = vec4(vec3(ambient + diffuse + specular), 1.0)
	//FragmentColor = vec4(vec3(ambient + diffuse + specular), 1.0) + texture(tex, exTexCoord);
	//FragmentColor = texture(tex,exTexCoord);
}