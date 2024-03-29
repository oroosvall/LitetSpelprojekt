#include "EchoLocation.h"
#include "../map.h"

EchoLocation::EchoLocation(glm::vec2 firstPos, glm::vec2 dir)
{
	direction = dir;
	initPos = firstPos; 
	moveTo(initPos.x, initPos.y);
	deathTimer = 3.5f;
	audibleDistance = 10.0f;
	alive = true;
	speed = 4.0f + GameConfig::get().configDifficulty * 2;

	contentIndex = EnemyID::echo_of_medivh;

	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 1, 1);

	if (dir.y > 0.0f)
		rotateTo(0, 0, acos(dir.x));
	else
		rotateTo(0, 0, -acos(dir.x));
}

void EchoLocation::init()
{
	//Not needed since it's a visitor-only projectile
}

int EchoLocation::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	translate(direction.x*speed*deltaTime, direction.y*speed*deltaTime);
	collideRect->update();
	deathTimer -= 1.0*deltaTime;
	if (deathTimer < FLT_EPSILON)
	{
		alive = false;
		return 1;
	}
	if (alive)
		return 0;
}

void EchoLocation::hit(int damage, bool playerRightOfEnemy)
{
}