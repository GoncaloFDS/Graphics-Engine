#include "Mesh.h"

//////////////////////////////FIXME
bool isOpenGlError() {

	bool isError = false;
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		const GLubyte* errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGlError(const std::string error) {
	if (isOpenGlError()) {
		std::cerr << error << std::endl;
		std::cin.get();
		exit(EXIT_FAILURE);
	}
}

////////////////////////////////////////////////////////////////////////

void Mesh::loadMeshData(std::string& filename) {
	std::ifstream ifile(filename);
	std::string line;
	while(std::getline(ifile, line)) 
		parseLine(std::stringstream(line));
	TexcoordsLoaded = (TexcoordIdx.size() > 0);
	NormalsLoaded = (NormalIdx.size() > 0);
}

void Mesh::processMeshData() {
	for (unsigned int i = 0; i < VertexIdx.size(); i++) {
		unsigned int vi = VertexIdx[i];
		Vec3 v = VertexData[vi - 1];
		Vertices.push_back(v);
		if (TexcoordsLoaded) {
			unsigned int ti = TexcoordIdx[i];
			Vec2 t = TexcoordData[ti - 1];
			Texcoords.push_back(t);
		}
		if (NormalsLoaded) {
			unsigned int ni = NormalIdx[i];
			Vec3 n = NormalData[ni - 1];
			Normals.push_back(n);
		}
	}
}

void Mesh::freeMeshData() {
	VertexData.clear();
	TexcoordData.clear();
	NormalData.clear();
	VertexIdx.clear();
	TexcoordIdx.clear();
	NormalIdx.clear();
}

void Mesh::parseVertex(std::stringstream& sin) {
	Vec3 v;
	sin >> v;
	VertexData.push_back(v);
}

void Mesh::parseTexcoord(std::stringstream& sin) {
	Vec2 t;
	sin >> t;
	TexcoordData.push_back(t);
}

void Mesh::parseNormal(std::stringstream& sin) {
	Vec3 n;
	sin >> n;
	NormalData.push_back(n);
}

void Mesh::parseFace(std::stringstream& sin) {
	std::string token;
	for (int i = 0; i < 3; i++) {
		std::getline(sin, token, '/');
		if (token.size() > 0) VertexIdx.push_back(std::stoi(token));
		std::getline(sin, token, '/');
		if (token.size() > 0) TexcoordIdx.push_back(std::stoi(token));
		std::getline(sin, token, ' ');
		if (token.size() > 0) NormalIdx.push_back(std::stoi(token));
	}
}

void Mesh::parseLine(std::stringstream& sin) {
	std::string s;
	sin >> s;
	if (s.compare("v") == 0) parseVertex(sin);
	else if (s.compare("vt") == 0) parseTexcoord(sin);
	else if (s.compare("vn") == 0) parseNormal(sin);
	else if (s.compare("f") == 0) parseFace(sin);
}

void Mesh::createBuffers() {
	glGenVertexArrays(1, &Vao);
	glBindVertexArray(Vao);
	{
		glGenBuffers(1, &VboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vec3), &Vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);

		if (TexcoordsLoaded)
		{
			glGenBuffers(1, &VboTexcoords);
			glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
			glBufferData(GL_ARRAY_BUFFER, Texcoords.size() * sizeof(Vec2), &Texcoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(TEXCOORDS);
			glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), 0);
		}
		if (NormalsLoaded)
		{
			glGenBuffers(1, &VboNormals);
			glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
			glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Vec3), &Normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(NORMALS);
			glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	checkOpenGlError("ERROR: Could not create VAOs and VBOs.");
}

Mesh::Mesh(std::string& filename) {
	loadMesh(filename);
	createBuffers();
}

void Mesh::draw(Shader* shader) {
	glBindVertexArray(Vao);
	shader->use();
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)Vertices.size());

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGlError("ERROR: Could not draw scene.");
}

void Mesh::loadMesh(std::string& filename) {
	loadMeshData(filename);
	processMeshData();
	freeMeshData();
}

