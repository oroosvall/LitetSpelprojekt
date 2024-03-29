#include "GrimScythe.h"
#include "../map.h"

GrimScythe::GrimScythe(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(initPos.x, initPos.y);
	audibleDistance = 10.0f;
	speed = 7.0f + GameConfig::get().configDifficulty * 2;;

	contentIndex = EnemyID::grim_scythe;
	alive = true;

	state = -1;
	stateTimer = 1.0f;

	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 4, 4);
	scaleFactor(1.25f, 1.25f, 1.25f);
	rotateTo(0, 3.141592654f, 0);
	backwards = false;
}

void GrimScythe::init()
{
	moveTo(initPos.x, initPos.y);
	collideRect->update();
}

int GrimScythe::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	collideRect->update();
	rotateTo(0, 0, 3.141592654f*3.0f*deltaTime);
	soundTimer -= 1.0f*deltaTime;
	if (soundTimer <= 0)
	{
		Audio::getAudio().playSoundAtPos(SoundID::boss_grim_scythe, readPos(), audibleDistance, false);
		soundTimer = 2.0f;
	}
		
	if (state == -1)
	{
		stateTimer -= 1.0f*deltaTime;
		if (stateTimer < FLT_EPSILON)
		{
			state = 0;
			calcDir(0);
		}
	}
	else if (state == 0)
	{
		if (reachedDestination())
		{
			if (!backwards)
				calcDir((headingTo + 1) % 4);
			else
			{
				headingTo--;
				if (headingTo < 0)
					headingTo = 3;
				calcDir(headingTo);
			}
		}
		else
			translate(dirToFly.x*speed*deltaTime, dirToFly.y*speed*deltaTime);
	}
	return 0;
}

void GrimScythe::hit(int damage, bool playerRightOfEnemy){}

void GrimScythe::calcDir(int position) 
{
	glm::vec2 pos = glm::vec2(readPos());
	switch (position)
	{
	case -1:
		currentGoal = initPos;
		break;
	case 0: //Top left
		currentGoal.x = initPos.x - 12.0f;
		currentGoal.y = initPos.y;
		break;
	case 1: //Bot left
		currentGoal.x = initPos.x - 12.0f;
		currentGoal.y = initPos.y - 10.0f;
		break;
	case 2: //Bot right
		currentGoal.x = initPos.x + 12.0f;
		currentGoal.y = initPos.y - 10.0f;
		break;
	case 3: //Top right
		currentGoal.x = initPos.x + 12.0f;
		currentGoal.y = initPos.y;
		break;
	}
	headingTo = position;
	dirToFly = currentGoal - pos;
	dirToFly = normalize(dirToFly);
}

bool GrimScythe::reachedDestination()
{
	glm::vec3 pos = readPos();
	if (pos.x < currentGoal.x + 1.0f && pos.x > currentGoal.x - 1.0f &&
		pos.y < currentGoal.y + 1.0f && pos.y > currentGoal.y - 1.0f)
		return true;
	return false;
}

void GrimScythe::returnToMaster()
{
	calcDir(-1);
	backwards = !backwards;
}