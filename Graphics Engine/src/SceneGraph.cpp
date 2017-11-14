#include "SceneGraph.h"

SceneGraph::SceneGraph() {
	Root = new SceneNode();
}

SceneNode* SceneGraph::createNode() {
	SceneNode* child = new SceneNode();
	child->setParent(Root);
	this->ChildNodes.push_back(child);
	return child;
}

SceneNode* SceneGraph::getRoot() {
	return Root;
}

void SceneGraph::setCamera(Camera* c) {
	MainCamera = c;
}

Camera* SceneGraph::getCamera() {
	return MainCamera;
}

void SceneGraph::setShaderProgram(Shader* s) {
	ShaderProg = s;
}

void SceneGraph::update() {
	for(SceneNode* node : ChildNodes) {
		node->update();
	}
}

void SceneGraph::draw() {
	for (SceneNode* node : ChildNodes) {
		node->draw();
	}
}
