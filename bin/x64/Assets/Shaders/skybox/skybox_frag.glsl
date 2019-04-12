#version 330 core

in vec3 texCoords;
out vec4 FragmentColor;
uniform samplerCube skybox;

void main(){
	
	vec3 temp = vec3(texCoords.x, -texCoords.y, texCoords.z);
	FragmentColor = texture(skybox, temp);
	//FragmentColor = vec4(1,0,0,1);
}