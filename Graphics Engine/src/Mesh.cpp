#include "Mesh.h"
#include "Application.h"

void Mesh::loadMeshData(std::string& filename) {
	std::ifstream ifile(filename);
	std::string line;
	while(std::getline(ifile, line)) 
		parseLine(std::stringstream(line));
	TexcoordsLoaded = (TexcoordIdx.size() > 0);
	NormalsLoaded = (TexcoordIdx.size() > 0);
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

Mesh::Mesh(std::string& filename, const GLuint id) {
	VaoId = id;
	loadMesh(filename);
}

void Mesh::loadMesh(std::string& filename) {
	loadMeshData(filename);
	processMeshData();
	freeMeshData();
}

