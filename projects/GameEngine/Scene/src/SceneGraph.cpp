#include "SceneGraph.h"
#include "ShaderManager.h"


SceneGraph::SceneGraph()
{
	this->root = new SceneNode();
}


SceneGraph::~SceneGraph()
{

}

SceneNode* SceneGraph::getRoot()
{
	return (this->root);
}

SceneNode* SceneGraph::createNode()
{
	SceneNode* node = this->root->createNode();
	return node;
}

QuaternionCamera* SceneGraph::getCamera()
{
	return this->camera;
}

void SceneGraph::setCamera(QuaternionCamera* camera)
{
	this->camera = camera;
}



void SceneGraph::Draw(Vector3 LightPosition)
{
	//Matrix3 NormalMatrix = Matrix3::ConvertFromMatrix4(DrawMatrix).inverse().transpose();
		
	//this->root->shaderProgram->Enable();
	//if (this->root->shaderProgram->getUniform("cameraPos") != -1)
		//glUniform3fv(this->root->shaderProgram->getUniform("cameraPos"),1, this->FreeCamera->position.coordinates);
	
		//glUniformMatrix4fv(this->root->shaderProgram->getUniform("ViewMatrix"), 1, GL_FALSE, this->FreeCamera->GetCamera().data);
		//glUniformMatrix4fv(this->root->shaderProgram->getUniform("ProjectionMatrix"), 1, GL_FALSE, FreeCamera->GetProjectionMatrix().data);
	//if (this->root->shaderProgram->getUniform("LightPosition") != -1)
		//glUniform3fv(this->root->shaderProgram->getUniform("LightPosition"), 1, LightPosition.coordinates);
	this->root->Draw();
	this->root->shaderProgram->Disable();

	if (this->skybox != nullptr) {
		//glDepthMask(GL_FALSE);
		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);
		//std::cout << "Drawing skybox tree" << std::endl;
		this->skybox->skyboxShader->Enable();
		this->skybox->cube->Draw();
		//glBindVertexArray(this->skybox->tree->VaoId);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, this->skybox->skybox->textureId);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glEnable(GL_CULL_FACE);
		//glEnable(GL_DEPTH_TEST);
		//glDepthMask(GL_TRUE);
		this->skybox->skyboxShader->Disable();
	}
}

void SceneGraph::Release()
{
	this->root->Release();
	this->Destroy();
}

void SceneGraph::Destroy()
{
	delete(this);
}

void SceneGraph::setSkyBox(SkyBox * box)
{
	this->skybox = box;
}
