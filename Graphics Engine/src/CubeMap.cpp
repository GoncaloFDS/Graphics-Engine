#include "CubeMap.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CubeMap::CubeMap() {
	float points[] = {
		-10.0f,  10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		-10.0f,  10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f,  10.0f
	};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	Engine::checkOpenGlError("merda cube4");
}


bool CubeMap::loadCubeMapSide(GLuint texture, GLenum sideTarget, const char* fileName) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	int x, y, n;
	int forceChannels = 4;
	unsigned char*  image_data = stbi_load(
		fileName, &x, &y, &n, forceChannels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", fileName);
		return false;
	}
	// non-power-of-2 dimensions check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(stderr,
			"WARNING: image %s is not power-of-2 dimensions\n",
			fileName);
	}

	// copy image data into 'target' side of cube map
	glTexImage2D(sideTarget, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	free(image_data);
	Engine::checkOpenGlError("merda cube3");
	return true;
}

void CubeMap::createCubeMap(const char* front, const char* back, const char* top,
	const char* bottom, const char* left, const char* right) {
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texCube);

	loadCubeMapSide(texCube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
	loadCubeMapSide(texCube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
	loadCubeMapSide(texCube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
	loadCubeMapSide(texCube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
	loadCubeMapSide(texCube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
	loadCubeMapSide(texCube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	Engine::checkOpenGlError("merda cube2");
}

void CubeMap::draw() {
	glDepthMask(GL_FALSE);
	shader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texCube);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	Engine::checkOpenGlError("merda cube1");
}

void CubeMap::setShader(Shader* s) {
	shader = s;
}
