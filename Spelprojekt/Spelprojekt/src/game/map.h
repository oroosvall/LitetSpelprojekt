#ifndef MAP_H
#define MAP_H

#include "mapChunk.h"
#include "enemy.h"
#include <string>
#include <sstream>
#include <fstream>

class Player;

using namespace std;

class Map
{
private:
	MapChunk** chunks = 0;
	int width = 0;
	int height = 0;
	int* upDraw;
	int* lastUpDraw;
	int counter = 0;

public:
	Map(){};
	~Map();
	void init();
	int readSquareSize() const;
	int readSizeX() const;
	int readSizeY() const;
	MapChunk** getChunks() const;
	void SaveMap(int id);
	void LoadMap(int id, glm::vec2* savedPickups);
	int playerspawnX = 0;
	int playerspawnY = 0;

	int update(float deltaTime, Player* player);
	//data ref x = -1, y -1 = outofbounds
	void getChunkIndex(glm::vec3 pos, int* idX, int* idY);
	bool collideMap(Rect* test, glm::vec3 pos);
	glm::vec3 collideEnemies(Rect* test, glm::vec3 pos);
	bool collideShrine(Rect* test, glm::vec3 pos, Shrine*& currentSpawn);
	void attackEnemies(Rect* wpnRect, glm::vec3 playerpos, int damage);
	glm::vec3 mirrorWalk(Rect* pRect, glm::vec3 pPos);
	void setUpDraw(glm::vec3 pos);
	void setUpDraw3x2(glm::vec3 pos);
	void setUpDrawMainMenuCamera(glm::vec3 pos);
	void setUpDrawCredits(glm::vec3 pos);
	int* getUpDraw() const;
	void respawnCheck();
	void playerDiedSoRespawnEnemies();

	glm::vec3 getChunkMiddle(glm::vec3 playerpos);

	std::string getBoss(glm::vec3 playerpos, bool startBoss);

	void findNewHome(Enemy* orphan);

	void giveMeHealthPickup(Player* p, Rect* pRect);
	bool webbedUp(Rect* pRect, glm::vec3 pPos);
};

#endif