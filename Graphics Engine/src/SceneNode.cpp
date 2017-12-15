#include "SceneNode.h"
#include "MatrixFactory.h"
#include "Vec4.h"
#include "Quat.h"

SceneNode::SceneNode() : ModelUniform(-1), ColorUniform(-1), State(Vec3(0, 0, 0), Quat(0, Vec4(0, 0, 1, 1))) {
	//sanity check
	Parent = nullptr;
	LocalMatrix = MatrixFactory::identity();
	WorldMatrix = MatrixFactory::identity();
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

void SceneNode::setActive(bool b) {
	isActive = b;
}

void SceneNode::setTex(Texture* tex)
{
	this->tex = tex;
}

void SceneNode::setMaterial(Material* mat)
{
	this->material = mat;
}

void SceneNode::applyTranslation(Vec3 t) {
	LocalMatrix = MatrixFactory::translate(t) * LocalMatrix;
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
	if(!isActive) return;
	SceneNode* currentParent = Parent;
	while (ShaderProg == nullptr) {
		ShaderProg = currentParent->getShader();
		currentParent = currentParent->getParent();
		ModelUniform = ShaderProg->getUniform("ModelMatrix");
		ColorUniform = ShaderProg->getUniform("Color");
	}
	if (NodeMesh != nullptr) {
		Vec3 LightPosition = Vec3(0, 0, 5);
		ShaderProg->use();
		glUniformMatrix4fv(ModelUniform, 1, GL_TRUE, WorldMatrix.entry);
		glUniform4fv(ColorUniform, 1, Color.asArray());
		ShaderProg->use();
		if (ShaderProg->getUniform("LightPosition") != -1) {
			float light[3] = { LightPosition.x, LightPosition.y, LightPosition.z };
			glUniform3fv(ShaderProg->getUniform("LightPosition"), 1, light);
		}
		if(ShaderProg->getUniform("NormalMatrix") != -1)
		{
			auto NormalMatrix = MatrixFactory::transpose(MatrixFactory::inverse(Mat3(WorldMatrix)));
			glUniformMatrix3fv(ShaderProg->getUniform("NormalMatrix"), 1, GL_TRUE, NormalMatrix.entry);
		}
		if (tex != nullptr && this->ShaderProg->getUniform("tex") != -1) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->tex->TextureId);	
			glUniform1i(ShaderProg->getUniform("tex"), 0);
		}
		if(material != nullptr && this->ShaderProg->getUniform("mat.shininess") != -1)
		{
			glUniform1f(this->ShaderProg->getUniform("mat.shininess"), this->material->shininess);
			glUniform3f(this->ShaderProg->getUniform("mat.ambient"), this->material->ambient.x, this->material->ambient.y, this->material->ambient.z);
			glUniform3f(this->ShaderProg->getUniform("mat.diffuse"), this->material->diffuse.x, this->material->diffuse.y, this->material->diffuse.z);
			glUniform3f(this->ShaderProg->getUniform("mat.specular"), this->material->specular.x, this->material->specular.y, this->material->specular.z);
			glUniform3f(this->ShaderProg->getUniform("mat.emissive"), this->material->emissive.x, this->material->emissive.y, this->material->emissive.z);

		}
	
		NodeMesh->draw();
	}

	
	for (SceneNode* node : ChildNodes)
		node->draw();
}
