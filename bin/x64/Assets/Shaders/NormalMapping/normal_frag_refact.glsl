#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct TangentSpace{
	vec3 LightPos;
	vec3 ViewPos;
	vec3 FragPos;
};

uniform Material mat;
uniform vec3 LightPosition;
uniform mat4 ViewMatrix;
uniform mat3 NormalMat;
uniform sampler2D tex;
uniform sampler2D normalMap;
in vec3 exPosition;
in vec2 exTexCoord;
in TangentSpace tangentSpace;


out vec4 FragmentColor;



const vec3 ambient_light = vec3(0.025, 0.025, 0.025);
const vec3 diffuse_light = vec3(0.40, 0.40, 0.40);
const vec3 specular_light = vec3(0.80, 0.80, 0.80);


const float shininess_factor = 128.0;
const float k = 0.1;
const float lightRadius = 20;


vec3 GetNormalFromMap(in sampler2D normalMap,in vec2 exTexCoord){
	vec3 normal = texture(normalMap, exTexCoord).rgb;
	return normalize(normal * 2.0 - 1.0);
}

float attenuation(float r, float f, float d) {
  float denom = d / r + 1.0;
  float attenuation = 1.0 / (denom*denom);
  float t = (attenuation - f) / (1.0 - f);
  return max(t, 0.0);
}

float UDKAttenuation(float distance, float exponent){
	return 1/(pow(distance, exponent));
}

float LightAttenuation(float a,float b, float distance){
	return 1/(1 + a*distance + (b*distance*distance));
}


void main(){
	

//	vec3 normal = texture(normalMap, exTexCoord).rgb;

//	normal = normalize(normal * 2.0 - 1.0);
	
	vec3 N = GetNormalFromMap(normalMap, exTexCoord);
	vec3 L = normalize((tangentSpace.LightPos - tangentSpace.FragPos));
	vec3 E = normalize((tangentSpace.ViewPos - tangentSpace.FragPos));
	vec3 H = normalize(L + E);
	
	float distanceToLight = length(tangentSpace.LightPos - tangentSpace.FragPos);
	//float att = attenuation(40,0.1, length(tangentSpace.LightPos - tangentSpace.FragPos));
	//float att = UDKAttenuation(length(tangentSpace.LightPos - tangentSpace.FragPos),2);
	float att = LightAttenuation(0.01, 0,distanceToLight);
	

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
	FragmentColor = vec4(vec3(final_ambient_color + diffuse_component*final_diffuse_color*att + specular_component*final_specular_color*att), 1.0) + texture(tex, exTexCoord);

	//FragmentColor = vec4(diffuse_component * final_diffuse_color, 1.0); // only diffuse component
	//FragmentColor = vec4(specular_component * final_specular_color, 1.0); //only specular component
	
	//FragmentColor = vec4(vec3(ambient + diffuse + specular), 1.0)
	//FragmentColor = vec4(vec3(ambient + diffuse + specular), 1.0) + texture(tex, exTexCoord);
	//FragmentColor = texture(tex,exTexCoord);
}