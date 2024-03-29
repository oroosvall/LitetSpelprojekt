#include "gameObject.h"

int GameObject::bindWorldMat(GLuint* shaderProgram, GLuint* shaderuniform) const
{
	glProgramUniformMatrix4fv(*shaderProgram, *shaderuniform, 1, false, &worldMat[0][0]);
	return contentIndex;
}

void GameObject::init(int idi)
{
	contentIndex = idi;
}

glm::mat4* GameObject::getWorldMat()
{
	return &worldMat;
}

void GameObject::rotateTo(float x, float y, float z)
{
	glm::vec3 pos = glm::vec3(worldMat[0].w, worldMat[1].w, worldMat[2].w);

	//move to origo
	worldMat[0].w -= pos.x;
	worldMat[1].w -= pos.y;
	worldMat[2].w -= pos.z;

	//rot X
	worldMat *= glm::mat4(1, 0.0f, 0.0f, 0.0f,
						0.0f, cos(x), -sin(x), 0.0f,
						0.0f, sin(x), cos(x), 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
	//rot Y
	worldMat *= glm::mat4(cos(y), 0.0f, -sin(y), 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						sin(y), 0.0f, cos(y), 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);

	//rot Z
	worldMat *= glm::mat4(cos(z), -sin(z), 0.0f, 0.0f,
						sin(z), cos(z), 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);

	//move back to worldspace pos
	worldMat[0].w += pos.x;
	worldMat[1].w += pos.y;
	worldMat[2].w += pos.z;
}

void GameObject::rotateToX(float x)
{
	rotXfloatsave += x;

	glm::vec3 pos = glm::vec3(worldMat[0].w, worldMat[1].w, worldMat[2].w);

	//move to origo
	worldMat[0].w -= pos.x;
	worldMat[1].w -= pos.y;
	worldMat[2].w -= pos.z;

	//rot X
	worldMat *= glm::mat4(1, 0.0f, 0.0f, 0.0f,
		0.0f, cos(x), sin(x), 0.0f,
		0.0f, -sin(x), cos(x), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);

	//move back to worldspace pos
	worldMat[0].w += pos.x;
	worldMat[1].w += pos.y;
	worldMat[2].w += pos.z;
}

void GameObject::rotateToXSNAP(float xin)
{
	glm::vec3 pos = glm::vec3(worldMat[0].w, worldMat[1].w, worldMat[2].w);
	glm::vec3 scale = glm::vec3(worldMat[0].x, worldMat[1].y, worldMat[2].z);
	
	//move to origo
	worldMat[0].w -= pos.x;
	worldMat[1].w -= pos.y;
	worldMat[2].w -= pos.z;

	//scale to 1
	worldMat[0].x *= (1.0f / scale.x);
	worldMat[1].y *= (1.0f / scale.y);
	worldMat[2].z *= (1.0f / scale.z);

	rotXfloatsaveTEMP += xin;
	int Octs = (int)(rotXfloatsaveTEMP / (3.141592653f * 0.5f));
	float x = float(Octs) * 3.141592653f * 0.5f;

	rotXfloatsave = x;
	
	//rot X
	worldMat *= glm::mat4(1, 0.0f, 0.0f, 0.0f,
		0.0f, cos(x), sin(x), 0.0f,
		0.0f, -sin(x), cos(x), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	//scale back
	worldMat[0].x *= scale.x;
	worldMat[1].y *= scale.y;
	worldMat[2].z *= scale.z;


	//move back to worldspace pos
	worldMat[0].w += pos.x;
	worldMat[1].w += pos.y;
	worldMat[2].w += pos.z;
}

void GameObject::rotateToY(float y)
{
	rotYfloatsave += y;

	glm::vec3 pos = glm::vec3(worldMat[0].w, worldMat[1].w, worldMat[2].w);

	//move to origo
	worldMat[0].w -= pos.x;
	worldMat[1].w -= pos.y;
	worldMat[2].w -= pos.z;

	//rot Z
	worldMat *= glm::mat4(cos(y), 0.0f, -sin(y), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin(y), 0.0f, cos(y), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	//move back to worldspace pos
	worldMat[0].w += pos.x;
	worldMat[1].w += pos.y;
	worldMat[2].w += pos.z;
}

void GameObject::rotateToYSNAP(float yin)
{
	
}

void GameObject::rotateToXY(float x, float y)
{
	rotateToX(x);
	rotateToY(y);
}

void GameObject::rotateToXYSNAP(float x, float y)
{
	rotateToXSNAP(x);
	rotateToYSNAP(y);
}

void GameObject::rotateToZ(float z)
{
	rotZfloatsave += z;

	glm::vec3 pos = glm::vec3(worldMat[0].w, worldMat[1].w, worldMat[2].w);

	//move to origo
	worldMat[0].w -= pos.x;
	worldMat[1].w -= pos.y;
	worldMat[2].w -= pos.z;

	//rot Z
	worldMat *= glm::mat4(cos(z), -sin(z), 0.0f, 0.0f,
						sin(z), cos(z), 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);

	//move back to worldspace pos
	worldMat[0].w += pos.x;
	worldMat[1].w += pos.y;
	worldMat[2].w += pos.z;
}

void GameObject::rotateToZSNAP(float z)
{
	rotZfloatsave += z;

	glm::vec3 pos = glm::vec3(worldMat[0].w, worldMat[1].w, worldMat[2].w);

	//move to origo
	worldMat[0].w -= pos.x;
	worldMat[1].w -= pos.y;
	worldMat[2].w -= pos.z;

	//rot Z
	worldMat *= glm::mat4(cos(z), -sin(z), 0.0f, 0.0f,
		sin(z), cos(z), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	//move back to worldspace pos
	worldMat[0].w += pos.x;
	worldMat[1].w += pos.y;
	worldMat[2].w += pos.z;
}

void GameObject::translate(float x, float y)
{
	worldMat[0].w += x;
	worldMat[1].w += y;
}

void GameObject::translate(float x, float y, float z)
{
	worldMat[0].w += x;
	worldMat[1].w += y;
	worldMat[2].w += z;
}

void GameObject::translateEDITOR(float x, float y, float z)
{
	worldMat[0].w += x;
	worldMat[1].w += y;
	worldMat[2].w += z;
	posXfloatsave = worldMat[0].w;
	posYfloatsave = worldMat[1].w;
	posZfloatsave = worldMat[2].w;
}

void GameObject::translateSNAP(float x, float y, float z)
{
	if (posXfloatsave < -99900.0f)
		posXfloatsave = worldMat[0].w;
	if (posYfloatsave < -99900.0f)
		posYfloatsave = worldMat[1].w;
	if (posZfloatsave < -99900.0f)
		posZfloatsave = worldMat[2].w;

	posXfloatsave += x;
	posYfloatsave += y;
	posZfloatsave += z;

	worldMat[0].w = (int)posXfloatsave;
	worldMat[1].w = (int)posYfloatsave;
	worldMat[2].w = (int)posZfloatsave;

}

void GameObject::translateSNAPXY(float x, float y)
{
	if (posXfloatsave < -99900.0f)
		posXfloatsave = worldMat[0].w;
	if (posYfloatsave < -99900.0f)
		posYfloatsave = worldMat[1].w;

	posXfloatsave += x;
	posYfloatsave += y;

	worldMat[0].w = (int)posXfloatsave;
	worldMat[1].w = (int)posYfloatsave;
}

void GameObject::translateSNAPZ(float z)
{
	if (posZfloatsave < -99900.0f)
		posZfloatsave = worldMat[2].w;

	posZfloatsave += z;

	worldMat[2].w = (int)posZfloatsave;

}

void GameObject::moveTo(glm::vec3 target)
{
	worldMat[0].w = target.x;
	worldMat[1].w = target.y;
	worldMat[2].w = target.z;
}

void GameObject::moveTo(float x, float y)
{
	worldMat[0].w = x;
	worldMat[1].w = y;
}

void GameObject::moveTo(float x, float y, float z)
{
	worldMat[0].w = x;
	worldMat[1].w = y;
	worldMat[2].w = z;
}

void GameObject::moveToXY(float x, float y)
{
	worldMat[0].w = x;
	worldMat[1].w = y;
}

void GameObject::moveToZ(float z)
{
	worldMat[2].w = z;
}

glm::vec3 GameObject::readPos() const
{
	return glm::vec3(worldMat[0].w, worldMat[1].w, worldMat[2].w);
}

void GameObject::scaleUniformAD(float val)
{
	worldMat[0].x += val;
	worldMat[1].y += val;
	worldMat[2].z += val;
}

void GameObject::scaleUniformFactor(float val)
{
	worldMat[0].x *= val;
	worldMat[1].y *= val;
	worldMat[2].z *= val;
}

void GameObject::scaleFactor(float x, float y, float z)
{
	worldMat[0].x *= x;
	worldMat[1].y *= y;
	worldMat[2].z *= z;
}

void GameObject::scaleAD(float x, float y, float z)
{
	worldMat[0].x += x;
	worldMat[1].y += y;
	worldMat[2].z += z;

	if (worldMat[0].x < 0.1f)
		worldMat[0].x = 0.1f;
	if (worldMat[1].y < 0.1f)
		worldMat[1].y = 0.1f;
	if (worldMat[2].z < 0.1f)
		worldMat[2].z = 0.1f;
}

void GameObject::scaleSNAP(float x, float y, float z)
{
	if (scaleXfloatsave < -99900.0f)
		scaleXfloatsave = worldMat[0].x;
	if (scaleYfloatsave < -99900.0f)
		scaleYfloatsave = worldMat[1].y;
	if (scaleZfloatsave < -99900.0f)
		scaleZfloatsave = worldMat[2].z;

	scaleXfloatsave += x;
	scaleYfloatsave += y;
	scaleZfloatsave += z;

	worldMat[0].x = (int)scaleXfloatsave;
	worldMat[1].y = (int)scaleYfloatsave;
	worldMat[2].z = (int)scaleZfloatsave;

	if (worldMat[0].x < 1.0f)
		worldMat[0].x = 1.0f;
	if (worldMat[1].y < 1.0f)
		worldMat[1].y = 1.0f;
	if (worldMat[2].z < 1.0f)
		worldMat[2].z = 1.0f;
}

int GameObject::update(float deltaTime)
{
	return 0;
}

int GameObject::returnID() const
{
	return contentIndex;
}

void GameObject::coppyMat(GameObject* obj)
{
	this->worldMat = obj->worldMat;
	this->posXfloatsave = obj->posXfloatsave;
	this->posYfloatsave = obj->posYfloatsave;
	this->posZfloatsave = obj->posZfloatsave;
	this->scaleXfloatsave = obj->scaleXfloatsave;
	this->scaleYfloatsave = obj->scaleYfloatsave;
	this->scaleZfloatsave = obj->scaleZfloatsave;
	this->rotXfloatsave = obj->rotXfloatsave;
	this->rotYfloatsave = obj->rotYfloatsave;
	this->rotZfloatsave = obj->rotZfloatsave;
	this->rotXfloatsaveTEMP = obj->rotXfloatsaveTEMP;
	this->rotYfloatsaveTEMP = obj->rotYfloatsaveTEMP;
	this->rotZfloatsaveTEMP = obj->rotZfloatsaveTEMP;
}

void GameObject::setWorldMat(glm::mat4* mat)
{
	worldMat = *mat;
}