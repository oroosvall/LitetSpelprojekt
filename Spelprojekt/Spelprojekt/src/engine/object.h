#ifndef OBJECT_H
#define OBJECT_H

#include "../GameConfig.h"
#include "../DebugLog.h"

#include <string>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "../stbImage/stb_image.h"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class Object
{
private:
	
	bool vertexHost = false;
	GLuint textureId = 0;
	bool textureHost = false;
	int TEXTUREINDEXOFFSET = 0;
	int faceCount = -1;
	float TexscaleX = 1;
	float TexscaleY = 1;
	int SCREENWIDTH = GameConfig::get().configResX;
	int SCREENHEIGHT = GameConfig::get().configResY;

	//temprary implementations
	bool loadVert(std::string path);
	bool loadBMP(std::string path); 
	bool loadPNG(std::string path);

public:
	GLuint vertexData = 0;
	GLuint indexBuffer = 0;
	GLuint vertexAttribute = 0;

	struct TriangleVertex
	{
		float x, y, z;
		float u, v;
	};

	std::vector<TriangleVertex> vert;
	std::vector<TriangleVertex> uv;
	std::vector<GLushort> Indices;

	int	vertcount = 0;
	int size = 0;
	int state = 0;

	Object() {}
	Object(std::string pathVert, std::string pathTex, Object* obj = 0, bool copyVert = false, bool copyTex = false);
	Object(const Object& obj);
	~Object();
	void bind();
	void bindTexOnly();
	void bindVertOnly();
	int getFaces();
	void updateVAO(std::vector<TriangleVertex> someVerts, std::vector<GLushort> someIndices);
	float scaleX();
	float scaleY();
};

#endif