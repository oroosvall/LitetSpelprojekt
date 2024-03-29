#include "spider.h"

Spider::Spider(glm::vec2 firstPos)
{
	initPos = firstPos;
	moveTo(firstPos.x, firstPos.y);
	contentIndex = 4;
	alive = true;
	if (GameConfig::get().configDifficulty == GameConfig::Casual)
		jumpTimer = 3.0f;
	else if (GameConfig::get().configDifficulty == GameConfig::Hardcore)
		jumpTimer = 2.0f;
	else if (GameConfig::get().configDifficulty == GameConfig::DmonInHell)
		jumpTimer = 1.0f;
	ceiling = true;
	jumping = false;
	lastFacing = facingRight = true;
	health = 2;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.9f, 0.9f);
	speed = glm::vec2(1, 0.0);
	speed.x += GameConfig::get().configDifficulty;
	audibleDistance = 2.5f;

	invulnTimer = 0.0f;

	rotateTo(3.14159265f, 0, 0);
}

Spider::Spider(Spider* copy)
{
	visitor = copy->visitor;
	contentIndex = 4;
	worldMat = copy->worldMat;
	initPos = glm::vec2(copy->readPos().x, copy->readPos().y);
	moveTo(initPos.x, initPos.y);
	alive = true;
	facingRight = copy->facingRight;
	lastFacing = copy->lastFacing;
	jumping = copy->jumping;
	jumpTimer = copy->jumpTimer;
	ceiling = copy->ceiling;
	health = copy->health;
	speed = copy->speed;
	collideRect = new Rect();
	collideRect->initGameObjectRect(&worldMat, 0.9f, 0.9f);

	invulnTimer = copy->invulnTimer;
}

Spider::~Spider()
{
}

void Spider::init()
{
	worldMat = glm::mat4(1);
	rotateTo(3.14159265f, 0, 0);
	moveTo(initPos.x, initPos.y);
	alive = true;
	if (GameConfig::get().configDifficulty == GameConfig::Casual)
		jumpTimer = 3.0f;
	else if (GameConfig::get().configDifficulty == GameConfig::Hardcore)
		jumpTimer = 2.0f;
	else if (GameConfig::get().configDifficulty == GameConfig::DmonInHell)
		jumpTimer = 1.0f;
	ceiling = true;
	jumping = false;
	lastFacing = facingRight = true;
	health = 2;
	speed = glm::vec2(1, 0.0);
	collideRect->update();

	invulnTimer = 0.0f;
}

int Spider::update(float deltaTime, Map* map, glm::vec3 playerPos)
{
	if (lastFacing != facingRight)
		rotateTo(0, 3.14159265f, 0);
	lastFacing = facingRight;

	if (ceiling)
	{
		if (facingRight)
		{
			translate(1.0f, 1.0f);
			if (!collidesWithWorld(map))
			{
				facingRight = false;
				translate(-1.0f, -1.0f);
			}
			else
			{
				translate(-1.0f, -1.0f);
				translate(speed.x*deltaTime, 0.0f);
				if (collidesWithWorld(map))
				{
					translate(-speed.x*deltaTime, 0.0f);
					facingRight = false;
				}
			}
		}
		else
		{
			translate(-1.0f, 1.0f);
			if (!collidesWithWorld(map))
			{
				facingRight = true;
				translate(1.0f, -1.0f);
			}
			else
			{
				translate(1.0f, -1.0f);
				translate(-speed.x*deltaTime, 0.0f);
				if (collidesWithWorld(map))
				{
					translate(speed.x*deltaTime, 0.0f);
					facingRight = true;
				}
			}
		}

		glm::vec3 pos = readPos();
		if (playerPos.y < pos.y && playerPos.x - pos.x < 2 && playerPos.x - pos.x > -2)
		{
			rotateTo(3.14159265f, 0, 0);
			Debug::DebugOutput("DROP IT!\n");
			ceiling = false;
			jumping = true;
			speed.x = 0.0f;
			Audio::getAudio().playSoundAtPos(SoundID::enemy_spider_jump, readPos(), audibleDistance, false); //enemy_spider_jump
		}
	}
	else
	{
		if (jumping)
		{
			speed.y -= 1.0f;
			if (speed.y < -25.0f)
				speed.y = -25.0f;
			translate(0, speed.y*deltaTime);
			if (collidesWithWorld(map))
			{	
				translate(0, -speed.y*deltaTime);
				glm::vec3 pos = readPos();
				if (speed.y < 0.0f)
				{
					if (GameConfig::get().configDifficulty == GameConfig::Casual)
						jumpTimer = 3.0f;
					else if (GameConfig::get().configDifficulty == GameConfig::Hardcore)
						jumpTimer = 2.0f;
					else if (GameConfig::get().configDifficulty == GameConfig::DmonInHell)
						jumpTimer = 1.0f;
					jumping = false;
					speed.x = 4.0f + GameConfig::get().configDifficulty;
					if (playerPos.x < pos.x)
						facingRight = false;
					else if (playerPos.x > pos.x)
						facingRight = true;
				}
				speed.y = 0.0f;
			}
			else if (facingRight)
			{
				translate(speed.x*deltaTime, 0);
				if (collidesWithWorld(map))
				{
					translate(-speed.x*deltaTime, 0);
					speed.x = 0.0f;
				}
			}
			else
			{
				translate(-speed.x*deltaTime, 0);
				if (collidesWithWorld(map))
				{
					translate(speed.x*deltaTime, 0);
					speed.x = 0.0f;
				}
			}
		}
		else
		{
			if (facingRight)
			{
				translate(1.0f, -1.0f);
				if (!collidesWithWorld(map))
				{
					facingRight = false;
					translate(-1.0f, 1.0f);
				}
				else
				{
					translate(-1.0f, 1.0f);
					translate(speed.x*deltaTime, 0.0f);
					if (collidesWithWorld(map))
					{
						translate(-speed.x*deltaTime, 0.0f);
						facingRight = false;
					}
				}
			}
			else
			{
				translate(-1.0f, -1.0f);
				if (!collidesWithWorld(map))
				{
					facingRight = true;
					translate(1.0f, 1.0f);
				}
				else
				{
					translate(1.0f, 1.0f);
					translate(-speed.x*deltaTime, 0.0f);
					if (collidesWithWorld(map))
					{
						translate(speed.x*deltaTime, 0.0f);
						facingRight = true;
					}
				}
			}
			jumpTimer -= 1.0f*deltaTime;
			if (jumpTimer < FLT_EPSILON)
			{
				glm::vec3 pos = readPos();
				if (facingRight && playerPos.x < pos.x)
					facingRight = false;
				else if (!facingRight && playerPos.x > pos.x)
					facingRight = true;
				if (playerPos.x - pos.x > 7 || playerPos.x - pos.x < -7)
					speed.x = 14.0;
				else
					speed.x = 8.0;
				speed.y = 14.0;
				jumping = true;
				Audio::getAudio().playSoundAtPos(SoundID::enemy_spider_jump, readPos(), audibleDistance, false); //enemy_spider_jump
			}
		}
	}
	if (invulnTimer > FLT_EPSILON)
	{
		invulnTimer -= 1.0f*deltaTime;
	}
	collideRect->update();
	return 0;
}

void Spider::hit(int damage, bool playerRightOfEnemy)
{
	if (invulnTimer < FLT_EPSILON)
	{
		invulnTimer = 0.7f;
		health -= damage;
		if (health <= 0)
		{
			alive = false;
			Audio::getAudio().playSoundAtPos(SoundID::enemy_spider_death, readPos(), audibleDistance, false); //enemy_spider_death
		}
		else
			Audio::getAudio().playSoundAtPos(SoundID::enemy_spider_hurt, readPos(), audibleDistance, false); //enemy_spider_hurt

		if (playerRightOfEnemy)
		{
			if (facingRight)
				facingRight = false;
		}
		else
		{
			if (!facingRight)
				facingRight = true;
		}
		speed.x = 12;
		speed.y = 15;
		jumping = true;
		if (ceiling)
		{
			ceiling = false;
			rotateTo(3.14159265f, 0, 0);
		}
	}
}
