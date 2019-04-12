#version 330 core

in vec3 inPosition;

out vec3 texCoords;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
void main(){
	texCoords = inPosition;
	vec4 temp = ProjectionMatrix * mat4(mat3(ViewMatrix)) * vec4(inPosition, 1.0);
	gl_Position = temp; //ProjectionMatrix*mat4(mat3(ViewMatrix)) * vec4(inPosition, 1.0);
	gl_Position.z = gl_Position.w - 0.00001;
}