#include "enemy.h"

Rect* Enemy::getRekt()
{
	return collideRect;
}

bool Enemy::isAlive()
{
	return alive;
}

Enemy::~Enemy()
{
	delete collideRect;
}

bool Enemy::isBlinking()
{
	return false;
}

std::string Enemy::isBoss()
{
	return "ChuckTesta";
}

bool Enemy::isInitiated()
{
	return true;
}

void Enemy::setVisitor()
{
	visitor = true;
}

bool Enemy::isVisitor()
{
	return visitor;
}

void Enemy::diePls()
{
	alive = false;
}