#ifndef MIRROR_HPP
#define MIRROR_HPP

#include "gameObject.h"
#include <gl/glew.h>

#include "../engine/GBuffer.h"

class Mirror : public GameObject
{
public:
	
	Mirror();
	~Mirror();

	void initGBuffer(Gbuffer &screenBuffer);

	bool isInitialized();

	void calculateNormal();
	void initBoss();
	void calcView();

	void render();

	Gbuffer mirrorBuffer;

	glm::mat4 viewMat;
	glm::mat4 projMat;


	void setTeleportLocation(glm::vec2 loc, glm::vec2 chunk)
	{
		teleportLocation = loc;
		chunkLocation = chunk;
	}
	glm::vec2 getTeleportLocation()
	{
		return glm::vec2(teleportLocation.x+chunkLocation.x*35, teleportLocation.y-chunkLocation.y*35);
	}

	glm::vec2 getChunkTeleport()
	{
		return chunkLocation;
	}

	void reflect(glm::vec3 campos);

	void setRect();
	Rect* getRekt()
	{
		return enterRect;
	}

	glm::mat4 wMat;

	GLuint targetId;

private:

	int sizeX, sizeY;

	float distance;

	bool initialized;

	glm::vec3 normal;
	glm::vec3 finalNormal;

	glm::vec2 teleportLocation;
	glm::vec2 chunkLocation;

	Rect* enterRect;


	// buffer stuff
	
	RenderTarget rTexture[4];
	GLuint mirrorShader;

	GLuint mirrorModelMatrix;
	GLuint mirrorP;
	GLuint mirrorV;

	GLuint unifromNormal;
	GLuint unifromWorld;

	static GLuint mirrorQuad;
	static GLuint mirrorVao;

};

#endif
