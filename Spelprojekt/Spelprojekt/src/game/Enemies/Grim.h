#ifndef GRIM_H
#define GRIM_H

#include "../Enemy.h"

class Grim : public Enemy
{
private:
	int mode; //0 = start, 1 = force, 2 = range, 3 = spark

	int state;
	float stateTimer;

	Rect* hurtRect;
	float invulnTimer;
	bool isInit;

	void fireBall(Map* map, glm::vec3 playerPos, bool rightEye);
public:
	~Grim();
	Grim(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, Map* map, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	bool isBlinking();
	bool isInitiated();
	std::string getType()
	{
		return "Grim";
	}

	glm::vec2 plsGiveBossPos();

	Rect* getHurtRekt()
	{
		return hurtRect;
	}

	glm::vec2 getHandPos();
};

#endif