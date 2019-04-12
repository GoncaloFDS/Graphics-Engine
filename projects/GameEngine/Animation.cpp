#include "Animation.h"



Animation::Animation()
{
	this->interpolationFactor = 1;
}

Animation::Animation(SceneNode * node, Quaternion final, float duration)
{
	this->node = node;
	this->qInitial = node->quat;
	this->qCurrent = this->qInitial;
	this->qFinal = (final * this->qInitial).NormalizeQuaternion();
	this->duration = duration;
	this->interpolationFactor = 1;
	this->state = AnimationState::Stopped;
}

Animation::Animation(SceneNode* node, Vector3 final, float duration)
{
	this->node = node;
	this->pInitial = Vector3(this->node->ModelMatrix.data[12], this->node->ModelMatrix.data[13], this->node->ModelMatrix.data[14]);
	this->pCurrent = this->pInitial;
	this->pFinal = final;
	this->duration = duration;
	this->interpolationFactor = 1;
//	this->type = AnimationType::Translation;
	this->state = AnimationState::Stopped;

}

Animation::Animation(SceneNode * node, Vector3 pFinal, Quaternion qFinal, float duration)
{
	this->node = node;
	this->qInitial = node->quat;
	this->qFinal = (qFinal * this->qInitial).NormalizeQuaternion();
	this->qCurrent = qInitial;
	this->pInitial = Vector3(this->node->ModelMatrix.data[12], this->node->ModelMatrix.data[13], this->node->ModelMatrix.data[14]);
	this->pCurrent = pInitial;
	this->pFinal = pFinal;
	this->duration = duration;
	this->interpolationFactor = 1;
	this->state = AnimationState::Stopped;
}


Animation::~Animation()
{
}

void Animation::animate(float delta)
{
	if (this->interpolationFactor > 0) {
		this->interpolationFactor -= (delta / this->duration);
		if (!this->isRewindable) {
			this->isRewindable = true;
		}
	}
	if (this->interpolationFactor < 0)
		this->interpolationFactor = 0;

	if(this->pCurrent != (this->pInitial + this->pFinal) && !this->isFinished && interpolationFactor >= 0)
	{
			this->pCurrent = Vector3(this->node->ModelMatrix.data[12], this->node->ModelMatrix.data[13], this->node->ModelMatrix.data[14]);
			Vector3 currentState = this->pFinal * (1 - this->interpolationFactor);
			Vector3 newPosition = this->pCurrent + (currentState - this->lastState);
			this->lastState = currentState;
			this->node->ModelMatrix.data[12] = newPosition.coordinates[0];
			this->node->ModelMatrix.data[13] = newPosition.coordinates[1];
			this->node->ModelMatrix.data[14] = newPosition.coordinates[2];
	}
	if (this->qCurrent != this->qFinal && !this->isFinished && interpolationFactor >= 0) {
		this->qCurrent = Quaternion::Slerp(this->qFinal, this->qInitial, this->interpolationFactor);
		this->node->quat = this->qCurrent;
	}
	this->node->setMatrix(this->node->ModelMatrix);

	if (interpolationFactor <= 0){
		this->isRewindable = true;
		this->isFinished = true;
		this->interpolationFactor = 0;
	}
	else
		this->isFinished = false;
}

void Animation::rewind(float delta)
{
	if(interpolationFactor < 1)
		this->interpolationFactor += delta / this->duration;
	if (interpolationFactor > 1) {
		interpolationFactor = 1;
	}
	if (this->pCurrent != this->pInitial && this->isRewindable && interpolationFactor <= 1)
	{
		Vector3 currentState = this->pFinal * (1 - this->interpolationFactor);
		Vector3 newPosition = this->pCurrent + (currentState - this->lastState);
		this->lastState = currentState;
		this->node->ModelMatrix.data[12] = newPosition.coordinates[0];
		this->node->ModelMatrix.data[13] = newPosition.coordinates[1];
		this->node->ModelMatrix.data[14] = newPosition.coordinates[2];
		this->pCurrent = Vector3(this->node->ModelMatrix.data[12], this->node->ModelMatrix.data[13], this->node->ModelMatrix.data[14]);
		
	}
	if (this->qCurrent != this->qInitial && this->isRewindable && interpolationFactor <= 1) {
			qCurrent = Quaternion::Slerp(qFinal, qInitial, interpolationFactor).NormalizeQuaternion();
			this->node->quat = qCurrent;
	}
	if (interpolationFactor >= 1) {
		this->isRewindable = false;
	}
	this->isFinished = false;
	this->node->setMatrix(this->node->ModelMatrix);
}
