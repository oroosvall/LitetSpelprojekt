#include "Shrine.h"

Shrine::Shrine(GameObject* object)
{
	runeMove = 0.0f;
	runeUp = true;
	runeTaken = false;
	this->object = object;
	if (object->returnID() == 2)
	{
		rune = FLAME;
		runeObj = new GameObject();
		runeObj->init(4);
		glm::vec3 pos = object->readPos();
		runeObj->moveTo(pos.x, pos.y+0.1, pos.z + 0.5);
		runeObj->scaleFactor(0.5, 0.5, 0.5);
	}
	collision = new Rect();
	collision->initGameObjectRect(object->getWorldMat(), 1.0f, 2.0f);
}

Shrine::~Shrine()
{
	delete collision;
	delete object;
	delete runeObj;
}

void Shrine::resetRune()
{
	runeTaken = false;
}

Rect* Shrine::getRekt()
{
	return collision;
}

glm::vec2 Shrine::getPos()
{
	return glm::vec2(object->readPos());
}

int Shrine::getRune()
{
	if (!runeTaken)
	{
		runeTaken = true;
		return rune;
	}
	return NORUNE;
}

void Shrine::update(float deltaTime)
{
	glm::vec3 runePos = runeObj->readPos();
	if (runeMove > 0.5f)
		runeUp = false;
	if (runeMove < -0.5)
		runeUp = true;
	if (runeUp)
	{
		runeObj->moveTo(runePos.x, runePos.y + 0.1*deltaTime);
		runeMove += 1.0*deltaTime;
	}
	else
	{
		runeObj->moveTo(runePos.x, runePos.y - 0.1*deltaTime);
		runeMove -= 1.0*deltaTime;
	}
	runeObj->rotateTo(0, 1.0*deltaTime, 0);
}