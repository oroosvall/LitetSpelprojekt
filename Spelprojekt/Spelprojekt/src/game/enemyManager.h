#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <string>
#include <fstream>

#include "enemy.h"

class MapChunk;
class Map;

using namespace std;

class EnemyManager
{
private:
	Enemy** flames; int flameCount, flameMax;
	Enemy** bats; int batCount, batMax;
	Enemy** spikes; int spikeCount, spikeMax;
	Enemy** cubes; int cubeCount, cubeMax;
	Enemy** spiders; int spiderCount, spiderMax;
	Enemy** ghosts; int ghostCount, ghostMax;
	Enemy** webs; int webCount, webMax;
	Enemy** webShots; int shotCount, shotMax;
	Enemy** spellbooks; int spellbookCount, spellbookMax;
	Enemy** missiles; int missileCount, missileMax;
	Enemy** deathboxes; int deathCount, deathMax;
	Enemy** echos; int echoCount, echoMax;
	
	Enemy* boss;

	//Exclusive for Grim
	Enemy* grimHands[2];
	Enemy* grimScythe;
	Enemy* grimLaser;

	void addBoss(string type, glm::vec2 pos);

	void addEnemy(string type, glm::vec2 pos);
	void expandEnemyArray(Enemy**& arr, int &oldMax);

	int visitorsToSendOut;
	Enemy** visitorHolder;
public:
	EnemyManager();
	EnemyManager(int enemies);
	~EnemyManager();
	int update(float deltaTime, MapChunk* chunk, glm::vec3 playerPos, Map* map);
	int size(string type);
	void init(ifstream &file, int xOffset, int yOffset);
	void save(ofstream* out, int xOffset, int yOffset);
	void initEmpty();
	void resetEnemies();
	int bindEnemy(int index, GLuint* shader, GLuint* uniform, string type);

	Enemy** getEnemies(string type);
	Enemy* getBoss();
	void startBoss();

	void addOutsider(Enemy* visitor, string type);
	Enemy** getVisitors()
	{
		return visitorHolder;
	}
};

#endif