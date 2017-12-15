#version 400

in vec3 texcoords;
uniform samplerCube cubeTexture;
out vec4 outColor;

void main() {
	outColor = texture(cubeTexture, texcoords);
}