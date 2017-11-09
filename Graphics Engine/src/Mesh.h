#pragma once
#include <GL/glew.h>
#include <vector>
#include "Vec3.h"
#include "Vec2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


class Mesh{
	std::vector<Vec3> Vertices, VertexData;
	std::vector<Vec2> Texcoords, TexcoordData;
	std::vector<Vec3> Normals, NormalData;
	std::vector<unsigned int> VertexIdx, TexcoordIdx, NormalIdx;
	bool TexcoordsLoaded = false;
	bool NormalsLoaded = false;

	GLuint VaoId;
	GLuint VboVertices, VboTexcoords, VboNormals;


	void loadMeshData(std::string& filename);
	void processMeshData();
	void freeMeshData();

	void parseVertex(std::stringstream& sin);
	void parseTexcoord(std::stringstream& sin);
	void parseNormal(std::stringstream& sin);
	void parseFace(std::stringstream& sin);
	void parseLine(std::stringstream& sin);

public:
	Mesh(std::string& filename, GLuint id);
	Mesh() = default;
	~Mesh() = default;
	
	void loadMesh(std::string& filename);
};


