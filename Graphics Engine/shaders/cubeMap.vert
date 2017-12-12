#version 400

in vec3 vp;
uniform mat4 ProjectionMatrix, ViewMatrix;
out vec3 texcoords;

void main(void) {
	texcoords = vp;
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(vp, 1.0);
}