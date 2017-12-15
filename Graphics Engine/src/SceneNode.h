#pragma once
#include "Mesh.h"
#include "Mat4.h"
#include "Vec4.h"
#include "NodeState.h"
#include "Material.h"

class SceneNode {
private:
	SceneNode* Parent;
	Texture* tex;
	Material* material;
	std::vector<SceneNode*> ChildNodes;
	Mat4 LocalMatrix;
	Mat4 WorldMatrix;
	Mesh* NodeMesh;
	Shader* ShaderProg;
	GLint ModelUniform;
	GLint ColorUniform;
	Vec4 Color;
	bool isActive = true;

public:
	NodeState State;
	SceneNode();
	
	SceneNode* createNode();
	void setMesh(Mesh* mesh);
	void setMatrix(Mat4 m);
	void setState(const Vec3 pos, Quat quat);
	void transformLocalMatrix(Mat4 m);
	void setParent(SceneNode* parent);
	SceneNode* getParent() const;
	void setShader(Shader* s);
	Shader* getShader() const;
	Mat4 getWorldMatrix() const;
	void setColor(Vec4 c);
	void setActive(bool b);
	void setTex(Texture* tex);
	void setMaterial(Material* mat);
	void applyTranslation(Vec3 t);
	void applyRotation(Quat q);

	void update();
	void draw();
};

