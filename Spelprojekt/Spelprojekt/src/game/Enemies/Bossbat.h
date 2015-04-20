#ifndef BOSSBAT_H
#define BOSSBAT_H

#include "../Enemy.h";

class Bossbat : public Enemy
{
private:
	float movementScale;
	float speed;
	bool collidesWithWorld(MapChunk* chunk);
public:
	Bossbat(glm::vec2 firstPos);
	void init();
	int update(float deltaTime, MapChunk* chunk);
	void hit(int damage, bool playerRightOfEnemy);
};

#endif