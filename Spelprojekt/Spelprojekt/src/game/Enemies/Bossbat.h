#ifndef BOSSBAT_H
#define BOSSBAT_H

#include "../Enemy.h";

class Bossbat : public Enemy
{
private:
	MapChunk* chunk;

	float movementScale;
	float speed;
	bool collidesWithWorld(MapChunk* chunk);
	bool slow;
	float invulnTimer;
	bool isInit;

	float batTimer;
	int batsToSpawn;
	void spawnBat(MapChunk* chunk, float deltaTime);

	float chargeTimer;
	bool charging;
	glm::vec3 chargePos;
	glm::vec3 returnPos;
	bool hasTurned;
public:
	Bossbat(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, Map* chunk, glm::vec3 playerPos);
	void hit(int damage, bool playerRightOfEnemy);
	bool isBlinking();
	bool isInitiated();
	std::string getType()
	{
		return "Bossbat";
	}

	glm::vec2 plsGiveBossPos();
};

#endif