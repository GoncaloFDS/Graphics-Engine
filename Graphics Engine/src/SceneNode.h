#pragma once
#include "Mesh.h"
#include "Mat4.h"
#include "Vec4.h"

class SceneNode {
private:
	SceneNode* Parent;
	std::vector<SceneNode*> ChildNodes;
	Mat4 LocalMatrix;
	Mat4 WorldMatrix;
	Mesh* NodeMesh;
	Shader* ShaderProg;
	GLint ModelUniform;
	GLint ColorUniform;
	Vec4 Color;

public:
	SceneNode();
	
	SceneNode* createNode();
	void setMesh(Mesh* mesh);
	void setMatrix(Mat4 m);
	void transformLocalMatrix(Mat4 m);
	void setParent(SceneNode* parent);
	SceneNode* getParent() const;
	void setShader(Shader* s);
	Shader* getShader() const;
	Mat4 getWorldMatrix() const;
	void setColor(Vec4 c);

	void update();
	void draw();
};

