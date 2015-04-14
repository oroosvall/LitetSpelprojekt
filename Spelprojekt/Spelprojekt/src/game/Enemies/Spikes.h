#ifndef SPIKES_H
#define SPIKES_H

#include "../Enemy.h"

class Spikes : public Enemy
{
private:
public:
	Spikes(glm::vec2 firstPos);
	void init();
	int update(float deltaTime);
	void hit(int damage);
	void getContentIndices(int &state1, int &state2, float &interpol);
};

#endif