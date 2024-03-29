#ifndef GAME_H
#define GAME_H

#include "../UserInput.h"
#include "../Gamepad.h"
#include "../GUI.h"
#include "../GameConfig.h"

#include "edit.h"
#include "../engine/engine.h"
#include "../engine/ContentManager.h"
#include "../engine/object.h"
#include "../engine/ObjectManager.h"
#include "../engine/updateAnimCheck.h"

#include "player.h"
#include "enemyManager.h"
#include "map.h"

#include <GLFW/glfw3.h>


enum gameState
{
	MENU,
	PLAY,
	INTRO,
	EDIT,
	PAUSE,
	CONTINUE,
	SETTINGS_MAIN,
	SETTINGS_AUDIO,
	SETTINGS_GRAPHICS,
	SETTINGS_TOGGLE_SOUND,
	SETTINGS_TOGGLE_RES,
	CREDITS,
	DIFFICULTY
};

class Game : public Align_16
{
private:
	vec3 lastdirFPS = vec3(1, 0, 0);
	gameState current = gameState(-1);
	gameState last = MENU;
	Engine* engine = 0;
	ContentManager* content = 0;
	Player* player = 0;
	UpdateAnimCheck* updateAnimCheck = 0;
	Map* map = 0;
	UserInput* in = 0;
	Gamepad* gamePad = 0;
	GUI* gui = 0;
	Edit* edit = 0;
	double lastX = 0.0f;
	double lastY = 0.0f;
	double xpos = 0.0f;
	double ypos = 0.0f;
	bool cameraFollow = false;
	mat4* viewMat = 0;
	int finalbossFreeze = 0;
	int finalbossFreezeTimer = 350;
	bool finalbossToCredits = false;

	GLFWwindow* windowRef;

	bool inBossRoom = false;
	glm::vec3 bossRoomMiddle;

	Progress playerProgress;
	glm::vec2 savedStartPos;
	
	void initSettings();
	void toggleFullscreen();
	void saveSettings();
	void checkForSave();
	void saveGame();

	glm::vec2 savedPickups[12];
	int savePickupNr = 0;

	void cameraUpdate();
	void cameraUpdateCredits();
	vec3 lastpos;
	vec3 lastposCredits;
	float speed = 0.03f;
	bool playerintrorun = false;


	unsigned __int64 frame;

	bool freeze;

public:
	Game() {};
	~Game();
	void init(GLFWwindow* windowRef);
	void mainLoop();
	void update(float deltaTime);
	void readInput(float deltaTime);
	void buttonEvents(int buttonEv);
};

#endif