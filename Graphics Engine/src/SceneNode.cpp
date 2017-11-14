#include "SceneNode.h"
#include "MatrixFactory.h"
#include "Vec4.h"

SceneNode::SceneNode() {
	//sanity check
	Parent = nullptr;
	LocalMatrix = MatrixFactory::Identity();
	WorldMatrix = MatrixFactory::Identity();
	NodeMesh = nullptr;
	ShaderProg = nullptr;
}

SceneNode* SceneNode::createNode() {
	SceneNode* child = new SceneNode();
	child->setParent(this);
	this->ChildNodes.push_back(child);
	return child;
}

void SceneNode::setMesh(Mesh* mesh) {
	NodeMesh = mesh;
}

void SceneNode::setMatrix(Mat4 m) {
	LocalMatrix = m;
}

void SceneNode::setParent(SceneNode* parent) {
	Parent = parent;
}

SceneNode * SceneNode::getParent() {
	return Parent;
}

void SceneNode::setShader(Shader* s) {
	ShaderProg = s;
}

Shader* SceneNode::getShader() {
	return ShaderProg;
}

Mat4 SceneNode::getWorldMatrix() {
	return WorldMatrix;
}

void SceneNode::update() {
	WorldMatrix = Parent->getWorldMatrix() * LocalMatrix;
	

	for (SceneNode* node : ChildNodes)
		node->update();
}

void SceneNode::draw() {
	SceneNode* currentParent = Parent;
	while (ShaderProg == nullptr) {
		ShaderProg = currentParent->getShader();
		currentParent = currentParent->getParent();
	}
	ShaderProg->use();
	GLint modelUniform = ShaderProg->getUniform("ModelMatrix");
	GLint colorUniform = ShaderProg->getUniform("Color");
	glUniformMatrix4fv(modelUniform, 1, GL_TRUE, WorldMatrix.entry);
	glUniform4fv(colorUniform, 1, Vec4(0.7, 0.4, 0.3, 1).asArray());
	NodeMesh->draw(ShaderProg);
	for (SceneNode* node : ChildNodes)
		node->draw();
}
