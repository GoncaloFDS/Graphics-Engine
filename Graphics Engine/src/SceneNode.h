#pragma once
#include "Mesh.h"
#include "Mat4.h"

class SceneNode {
private:
	SceneNode* Parent;
	std::vector<SceneNode*> ChildNodes;
	Mat4 LocalMatrix;
	Mat4 WorldMatrix;
	Mesh* NodeMesh;
	Shader* ShaderProg;

public:
	SceneNode();
	
	SceneNode* createNode();
	void setMesh(Mesh* mesh);
	void setMatrix(Mat4 m);
	void setParent(SceneNode* parent);
	SceneNode* getParent();
	void setShader(Shader* s);
	Shader* getShader();
	Mat4 getWorldMatrix();

	void update();
	void draw();
};

