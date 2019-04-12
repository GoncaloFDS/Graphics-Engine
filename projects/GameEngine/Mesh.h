#pragma once
#ifndef MESH_H
#define MESH_H
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "Shader.h"
#include "Matrix4.h";
#include "QuaternionCamera.h"
#include "Texture.h"
#define VERTEX 0
#define TEXCOORDS 1
#define NORMALS 2
#define TANGENTS 3
#define BITANGENTS 4

typedef Vector3 Vertex;
typedef Vector2 TexCoordinates;
typedef Vector3 Normal;
class Mesh
{
public:
	Texture* tex;
	bool HasTexs, HasNormals, HasTangents, HasBiTangents;
	std::vector <Vertex> Vertices;
	std::vector <TexCoordinates> Texs;
	std::vector <Normal> Normals;
	std::vector <Vector4> Tangents;
	std::vector <Vector3> BiTangents;
	GLuint VaoId;
	Mesh();
	Mesh(std::string filepath);
	void FreeMeshData();
	void ProcessMesh();
	void Draw();
	~Mesh();
	
	
private:
	std::vector <Vertex> VerticesData;
	std::vector <TexCoordinates> TexsData;
	std::vector <Normal> NormalsData;
	std::vector <Vector4> TangentsData;
	std::vector <Vector4> BiTangentsData;
	std::vector <unsigned int> VerticesIdx, TexsIdx, NormalsIdx;
	void LoadFromFile(std::string filename);
	void ParseLine(std::stringstream& in);
	void parseVertexs(std::stringstream& in);
	void parseTextures(std::stringstream& in);
	void parseNormals(std::stringstream& in);
	void parseFaces(std::stringstream &in);
	void parseComments(std::stringstream &in);
	void CalculateTangents();

	GLuint VboVertices, VboTextures, VboNormals, VboTangents, VboBiTangents;
	bool Error = false;
	bool CreateBufferObjects();
	

};

#endif

