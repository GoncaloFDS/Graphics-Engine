#pragma once
#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H
#include <vector>
#include "SceneNode.h"
#include "QuaternionCamera.h"
#include "SkyBox.h"
class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();
	SceneNode* root;
	//Gets pointer to root node of this scenegraph.
	SceneNode* getRoot();
	//Creates a new node attached to the root node of the scenegraph.
	SceneNode* createNode();
	QuaternionCamera* camera;
	Camera* FreeCamera;
	//Gets the QuaternionCamera associated with this scenegraph.
	QuaternionCamera* getCamera();
	//Sets a QuaternionCamera to be used by the scenegraph.
	void setCamera(QuaternionCamera* camera);
	//Draws the scenegraph.
	void Draw(Vector3 LightPosition);
	//Releases the scenegraph and all its children nodes.
	void Release();
	void Destroy();
	SkyBox* skybox;
	void setSkyBox(SkyBox* box);
};

#endif