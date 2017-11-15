#include "SceneNode.h"
#include "MatrixFactory.h"
#include "Vec4.h"
#include "Quat.h"

SceneNode::SceneNode() : ModelUniform(-1), ColorUniform(-1), State(Vec3(0, 0, 0), Quat(0, Vec4(0, 0, 1, 1))) {
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

void SceneNode::setState(const Vec3 pos, Quat quat) {
	State.position = pos;
	State.quat = quat;
}

void SceneNode::transformLocalMatrix(Mat4 m) {
	LocalMatrix = m * LocalMatrix;
}

void SceneNode::setParent(SceneNode* parent) {
	Parent = parent;
}

SceneNode * SceneNode::getParent() const {
	return Parent;
}

void SceneNode::setShader(Shader* s) {
	ShaderProg = s;
	ShaderProg->use();
	ModelUniform = ShaderProg->getUniform("ModelMatrix");
	ColorUniform = ShaderProg->getUniform("Color");
	ShaderProg->detach();
}

Shader* SceneNode::getShader() const {
	return ShaderProg;
}

Mat4 SceneNode::getWorldMatrix() const {
	return WorldMatrix;
}

void SceneNode::setColor(const Vec4 c) {
	Color = c;
}

void SceneNode::applyTranslation(Vec3 t) {
	LocalMatrix = MatrixFactory::Translate(t) * LocalMatrix;
	State.position += t;
}

void SceneNode::applyRotation(Quat q) {
	State.quat = q;
}

void SceneNode::update() {
	WorldMatrix = Parent->getWorldMatrix() * LocalMatrix * State.quat.toMatrix();
	//std::cout << State.quat.toMatrix() << std::endl << std::endl;
	
	for (SceneNode* node : ChildNodes)
		node->update();
}

void SceneNode::draw() {
	SceneNode* currentParent = Parent;
	while (ShaderProg == nullptr) {
		ShaderProg = currentParent->getShader();
		currentParent = currentParent->getParent();
		ModelUniform = ShaderProg->getUniform("ModelMatrix");
		ColorUniform = ShaderProg->getUniform("Color");
	}
	if (NodeMesh != nullptr) {
		ShaderProg->use();
		glUniformMatrix4fv(ModelUniform, 1, GL_TRUE, WorldMatrix.entry);
		glUniform4fv(ColorUniform, 1, Color.asArray());
		NodeMesh->draw(ShaderProg);
	}
	for (SceneNode* node : ChildNodes)
		node->draw();
}
