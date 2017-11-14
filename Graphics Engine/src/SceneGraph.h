#pragma once
#include "Camera.h"
#include "SceneNode.h"

class SceneGraph {
	Camera* MainCamera;
	SceneNode* Root;
	Shader* ShaderProg;
	std::vector<SceneNode*> ChildNodes;

public:
	SceneGraph();

	SceneNode* createNode();
	SceneNode* getRoot();
	void setCamera(Camera* c);
	Camera* getCamera();
	void setShaderProgram(Shader* s);
	

	void update();
	void draw();
};
