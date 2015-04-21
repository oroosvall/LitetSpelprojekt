#include "Bossbat.h"
#include "../mapChunk.h"

Bossbat::Bossbat(glm::vec2 firstPos)
{
	scaleFactor(2, 2, 2);
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = false;
	isInit = false;
	facingRight = true;
	contentIndex = 1;
	health = 4;
	speed = 4.0f;
	slow = false;

	invulnTimer = 0.0f;
	movementScale = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1.8, 2);
}

void Bossbat::init()
{
	if (!isInit)
	{
		isInit = true;
		moveTo(initPos.x, initPos.y);
		invulnTimer = 0.0f;
		movementScale = 0.0f;
		if (!facingRight)
			rotateTo(0, 3.1415927f, 0);
		facingRight = true;
		alive = true;
		health = 4;
		slow = false;
		collideRect->update();
	}
	else
	{
		isInit = false;
		alive = false;
	}
}

int Bossbat::update(float deltaTime, MapChunk* chunk, glm::vec3 playerPos)
{
	glm::vec3 pos = readPos();
	if (invulnTimer > FLT_EPSILON)
	{
		invulnTimer -= 1.0*deltaTime;
		moveTo(pos.x, pos.y + speed*deltaTime);
		if (collidesWithWorld(chunk))
		{
			moveTo(pos.x, pos.y - speed*deltaTime);
		}
	}
	else
	{
		float distanceDown = pos.y - initPos.y;
		if (distanceDown > FLT_EPSILON)
		{
			moveTo(pos.x, pos.y - speed*deltaTime*(distanceDown / 10.0f));
			if (collidesWithWorld(chunk))
			{
				moveTo(pos.x, pos.y + speed*deltaTime*(distanceDown / 10.0f));
			}
		}
	}
	pos = readPos();
	if (facingRight)
	{
		if (movementScale < -1.0f)
			slow = true;
		else if (movementScale > 1.0f)
			slow = true;
		else
			slow = false;

		if (slow)
			moveTo(pos.x + speed*deltaTime * 0.5f, pos.y);
		if (!slow)
			moveTo(pos.x + speed*deltaTime, pos.y);
		movementScale += 1.0*deltaTime;

		if (collidesWithWorld(chunk))
		{
			movementScale -= 1.0*deltaTime;
			if (slow)
			{
				moveTo(pos.x - speed*deltaTime * 0.5, pos.y);
				facingRight = false;
			}
			else
			{
				moveTo(pos.x - speed*deltaTime, pos.y);
				facingRight = false;
			}
		}
		
		if (movementScale > 1.5f)
			facingRight = false;
		if (!facingRight)
			rotateTo(0, 3.1415927f, 0);
	}
	else
	{
		if (movementScale < -1.0f)
			slow = true;
		else if (movementScale > 1.0f)
			slow = true;
		else
			slow = false;

		if (slow)
			moveTo(pos.x - speed*deltaTime * 0.5f, pos.y);
		if (!slow)
			moveTo(pos.x - speed*deltaTime, pos.y);
		movementScale -= 1.0*deltaTime;

		if (collidesWithWorld(chunk))
		{
			movementScale += 1.0*deltaTime;
			if (slow)
			{
				moveTo(pos.x + speed*deltaTime * 0.5, pos.y);
				facingRight = true;
			}
			else
			{
				moveTo(pos.x + speed*deltaTime, pos.y);
				facingRight = true;
			}
		}

		if (movementScale < -1.5f)
			facingRight = true;
		if (facingRight)
			rotateTo(0, 3.1415927f, 0);
	}
	return 0;
}

bool Bossbat::isInitiated()
{
	return isInit;
}

void Bossbat::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		health -= damage;
		if (health > 0)
		{
			invulnTimer = 1.0f;
			printf("Boss took damage \n");
		}
		else
			alive = false;
	}
}

bool Bossbat::collidesWithWorld(MapChunk* chunk)
{
	collideRect->update();
	return chunk->collide(collideRect);
}

bool Bossbat::isBlinking()
{
	if (invulnTimer > 0.0f)
	{
		int check = int(invulnTimer * 10);
		if (check % 3)
		{
			return true;
		}
	}
	return false;
}

std::string Bossbat::isBoss()
{
	return "Bossbat";
}