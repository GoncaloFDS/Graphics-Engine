#pragma once
#include <GL/glew.h>
#include <vector>
#include "Vec3.h"
#include "Vec2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Shader.h"
#include "Texture.h"

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

class Mesh {
	Texture* tex;
	std::vector<Vec3> Vertices, VertexData;
	std::vector<Vec2> Texcoords, TexcoordData;
	std::vector<Vec3> Normals, NormalData;
	std::vector<unsigned int> VertexIdx, TexcoordIdx, NormalIdx;
	bool TexcoordsLoaded = false;
	bool NormalsLoaded = false;

	GLuint Vao, Ebo;
	GLuint VboVertices, VboTexcoords, VboNormals;


	void loadMeshData(std::string& filename);
	void processMeshData();
	void freeMeshData();

	void parseVertex(std::stringstream& sin);
	void parseTexcoord(std::stringstream& sin);
	void parseNormal(std::stringstream& sin);
	void parseFace(std::stringstream& sin);
	void parseLine(std::stringstream& sin);

	void createBuffers();
	void destroyBuffers();
public:
	Mesh(std::string& filename);
	Mesh() = default;
	~Mesh() = default;

	void draw(Shader* shader);
	void setTex(Texture* tex);
	void loadMesh(std::string& filename);
};


