#include "Bossspider.h"
#include "../mapChunk.h"
#include "../map.h"
#include "Webshot.h"

Bossspider::Bossspider(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	alive = false;
	isInit = false;
	facingRight = true;
	contentIndex = EnemyID::spider;
	health = 6;
	speed = glm::vec2(8.0f, 0.0f);
	audibleDistance = 2.5f;

	invulnTimer = 0.0f;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 3.1, 2.8);
	hurtRect = new Rect();
	hurtRect->initGameObjectRect(&worldMat, 3.3, 3.2);

	currentMode = -1;
	webTimer = 0.0f;
	jumpTimer = -1.0f;
}

void Bossspider::init()
{
	if (!isInit)
	{
		worldMat = glm::mat4(1);
		scaleFactor(3, 3, 2.5);
		isInit = true;
		moveTo(initPos.x, initPos.y);
		invulnTimer = 0.0f;
		if (!facingRight)
			rotateTo(0, 3.14159265f, 0);
		facingRight = true;
		alive = true;
		health = 6;
		collideRect->update();
		hurtRect->update();

		currentMode = -1;
		webTimer = 2.0f;
		websToShoot = 0;
		jumpTimer = -1.0f;
	}
	else
	{
		isInit = false;
		alive = false;
	}
}

void Bossspider::howDoIShotWeb(glm::vec3 playerPos, Map* map)
{
	Webshot* pewpew = new Webshot(glm::vec2(readPos()));
	pewpew->setVisitor();
	glm::vec2 dir = glm::vec2(playerPos) - glm::vec2(readPos());
	dir.x = dir.x + (rand() % 5 - 2);
	dir.y = dir.y + (rand() % 5 - 2);
	dir = normalize(dir);
	pewpew->setDirection(dir);
	map->findNewHome(pewpew);
	delete pewpew;
	websToShoot--;
}

int Bossspider::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	if (invulnTimer > 0.0f)
		invulnTimer -= 1.0f*deltaTime;
	if (currentMode == -1) //Spawning
	{
		webTimer -= 1.0*deltaTime;
		if (webTimer < FLT_EPSILON)
			currentMode = 0;
	}
	else if (currentMode == 0) //Dropping from ceiling
	{
		speed.y -= 4.0;
		if (speed.y < -25)
			speed.y = -25;
		translate(0, speed.y*deltaTime);
		if (collidesWithWorld(map))
		{
			translate(0, -speed.y*deltaTime);
			speed.y = 0;
			currentMode = 1;
			jumpTimer = 7.0f;
			printf("Mode switch to 1\n");

		}
	}
	else if (currentMode == 1) //Rolling around at the speed of sound, got places to go, gotta follow my rainbow
	{
		translate(speed.x*deltaTime, 0);
		if (collidesWithWorld(map))
		{
			translate(-speed.x*deltaTime, 0);
			if (facingRight)
			{
				facingRight = false;
				rotateTo(0, 3.14159265f, 0);
			}
			else
			{
				facingRight = true;
				rotateTo(0, 3.14159265f, 0);
			}
			speed.x = -speed.x;
		}
		jumpTimer -= 1.0f*deltaTime;
		if (jumpTimer < FLT_EPSILON)
		{
			speed.y = 25.0f;
			currentMode = 2;
			printf("Modeswitch to 2\n");
		}
	}
	else if (currentMode == 2) //Jumping back up
	{
		speed.y -= 0.2;
		translate(0, speed.y*deltaTime);
		if (collidesWithWorld(map))
		{
			if (speed.y >= 0)
			{
				translate(0, -speed.y*deltaTime);
				currentMode = 3;
				webTimer = 1.0f;
				websToShoot = 2;
				printf("Modeswitch to 3\n");
			}
			else
			{
				translate(0, -speed.y*deltaTime);
				currentMode = 1;
				printf("Landed on ground after jump. Modeswitch to 1\n");
				jumpTimer = 5.0f;
			}
			speed.y = 0;
		}
	}
	else if (currentMode == 3) //Spiderman, spiderman, shoots whatever a spider can
	{
		webTimer -= 1.0*deltaTime;
		if (webTimer < FLT_EPSILON && websToShoot == 0)
		{
			currentMode = 0;
			printf("Modeswitch to 0\n");
		}
		else
		{
			if (webTimer < FLT_EPSILON)
			{
				howDoIShotWeb(playerPos, map);
				webTimer = 0.5f;
			}
		}
	}
	hurtRect->update();
	return 0;
}

bool Bossspider::isInitiated()
{
	return isInit;
}

void Bossspider::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		health -= damage;
		if (health > 0)
		{
			invulnTimer = 1.0f;
			printf("Boss took damage \n");
			Audio::getAudio().playSoundAtPos(SoundID::boss_bat_hurt, readPos(), audibleDistance, false);//boss_bat_hurt
			if (currentMode == 1)
			{
				jumpTimer = 1.5f;
				speed.y = 25.0f;
			}
		}
		else if (alive == true)
		{
			alive = false;
			printf("Boss is dead \n");
			Audio::getAudio().playSoundAtPos(SoundID::boss_bat_death, readPos(), audibleDistance, false);//boss_bat_death
		}
	}
}

bool Bossspider::isBlinking()
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

glm::vec2 Bossspider::plsGiveBossPos()
{
	return initPos;
}