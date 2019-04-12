#include "SceneNode.h"



SceneNode::SceneNode()
{
	this->ModelMatrix = Matrix4::IDENTITY;
	this->DrawMatrix = Matrix4::IDENTITY;
	this->quat = Quaternion(1, 0, 0, 0);
	this->textures = std::map<std::string, Texture*>();
}


SceneNode::~SceneNode()
{
}

void SceneNode::ChangeDirection(float angle, Vector4 axis)
{
	Quaternion tempQuat = Quaternion::FromAngleAxis(angle, axis).NormalizeQuaternion();
	this->quat = (tempQuat * this->quat).NormalizeQuaternion();	
	this->DrawMatrix = this->parent->DrawMatrix * this->GetModelMatrix() * this->quat.ToGLMatrix();
	this->Update();
}


void SceneNode::setActive(bool b)
{
	this->isActive = b;
}

void SceneNode::setMesh(Mesh* newMesh)
{
	this->mesh = newMesh;
}

Mesh* SceneNode::GetMesh()
{
	return this->mesh;
}

Matrix4 SceneNode::GetModelMatrix()
{
	return this->ModelMatrix;
}

void SceneNode::setMatrix(Matrix4 newMatrix)
{
	this->ModelMatrix = newMatrix;
	
	if (this->parent == nullptr)
	{
		this->DrawMatrix = this->ModelMatrix*this->quat.ToGLMatrix();
	}else
	{
		this->DrawMatrix = this->parent->DrawMatrix*this->ModelMatrix*this->quat.ToGLMatrix();
	}
	this->Update();
}


SceneNode* SceneNode::createNode()
{
	SceneNode * childrenNode = new SceneNode();
	childrenNode->setShaderProgram(this->shaderProgram);
	childrenNode->parent = this;
	childrenNode->DrawMatrix = childrenNode->GetModelMatrix() * this->DrawMatrix;
	this->children.push_back(childrenNode);
	return this->children.back();
}

void SceneNode::setShaderProgram(Shader* shader)
{
	this->shaderProgram = shader;
}

void SceneNode::Draw()
{
/*
	if (shaderProgram->GetUniform("NormalMatrix") > 0)
		glUniformMatrix3fv(shaderProgram->GetUniform("NormalMatrix"), 1, GL_FALSE, NormalMatrix.data);
*/
	if (!isActive) return;
	if(this->mesh != nullptr){
		this->shaderProgram->Enable();
		if (this->shaderProgram->getUniform("ModelMatrix") != -1) {
			glUniformMatrix4fv(this->shaderProgram->getUniform("ModelMatrix"), 1, GL_FALSE, DrawMatrix.data);
		}
		/*
		if (this->mesh->HasTexs && tex != nullptr && shaderProgram->getUniform("tex") != -1) {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, this->tex->TextureId);
			glUniform1i(shaderProgram->getUniform("tex"), 2);
			
			//glBindTexture(GL_TEXTURE_2D, 0);
		}
		if (this->mesh->HasTangents && tex2 != nullptr && shaderProgram->getUniform("normalMap") != -1) {
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, this->tex2->TextureId);
			glUniform1i(shaderProgram->getUniform("normalMap"), 3);

			//glBindTexture(GL_TEXTURE_2D, 0);
		}*/
		int textureUnit = 0;
		if (this->textures.size() > 0) {
			for (auto entry = this->textures.begin(); entry != this->textures.end(); ++entry) {
				if (this->shaderProgram->getUniform(entry->first.c_str()) != -1) {
					glActiveTexture(GL_TEXTURE2 + textureUnit);
					glBindTexture(GL_TEXTURE_2D, entry->second->TextureId);
					glUniform1i(this->shaderProgram->getUniform(entry->first.c_str()), 2 + textureUnit);
					textureUnit++;
				}
			}
		}


		if (this->material != nullptr && this->shaderProgram->getUniform("mat.shininess") != -1) {
			glUniform1fv(this->shaderProgram->getUniform("mat.shininess"),1, &this->material->shininess);
			glUniform3fv(this->shaderProgram->getUniform("mat.ambient"),1, this->material->ambient.coordinates);
			glUniform3fv(this->shaderProgram->getUniform("mat.diffuse"), 1, this->material->diffuse.coordinates);
			glUniform3fv(this->shaderProgram->getUniform("mat.specular"), 1, this->material->specular.coordinates);
			glUniform3fv(this->shaderProgram->getUniform("mat.emissive"), 1, this->material->emissive.coordinates);
		}

		mesh->Draw();
	}
	for(auto children: this->children)
	{
		children->Draw();
	}
}

void SceneNode::Update()
{
	for(auto &child : this->children)
	{
		child->DrawMatrix = this->DrawMatrix * child->GetModelMatrix() * child->quat.ToGLMatrix();
		child->Update();
	}
}

void SceneNode::Release()
{
	while (this->children.size() != 0) {
		std::cout << this << " Deleting children" << std::endl;
		this->children.back()->Release();
		this->children.pop_back();
	}
	std::cout << this << " Getting destroyed" << std::endl;
	this->Destroy();
}

void SceneNode::Destroy()
{
	delete(this);
}


void SceneNode::addTexture(std::string NameInShader, Texture * tex)
{
	this->textures[NameInShader] = tex;
}


