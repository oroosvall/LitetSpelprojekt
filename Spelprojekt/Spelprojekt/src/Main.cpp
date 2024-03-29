#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "game\game.h"

#include <ctime>

#include "DebugLog.h"

//#include <vld.h>

int main(int argv, char* argc[])
{

	// flags are append existing file, print to console, print to file
//#ifdef _DEBUG
	Debug::OpenDebugStream(false, true, true);
//#endif

	srand((unsigned int)time(0));

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GLFWwindow* wnd;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, false);

	// init graphics

	ifstream in;
	char* settings = "Config/settings.s";
	in.open(settings);
	if (in)
	{
		// default config settings
		GameConfig::get().configDifficulty = 0;
		GameConfig::get().configRenderGlow = true;
		GameConfig::get().configFirstperson = false;
		GameConfig::get().configFullscreen = false;
		GameConfig::get().configResX = 1080, GameConfig::get().configResY = 720;

		string line;
		string sub;
		stringstream ss;

		//fullscreen
		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		GameConfig::get().configFullscreen = atoi(sub.c_str()) ? 1 : 0;

		// read past first person mode
		getline(in, line);
		ss = stringstream(line);
		ss >> sub;

		//resolution
		getline(in, line);
		ss = stringstream(line);
		ss >> sub;
		GameConfig::get().configResX = atoi(sub.c_str());
		ss >> sub;
		GameConfig::get().configResY = atoi(sub.c_str());

		in.close();
	}
	
	if (GameConfig::get().configFullscreen) // fullscreen
		wnd = glfwCreateWindow(GameConfig::get().configResX, GameConfig::get().configResY, "ASUM PROJECT", glfwGetPrimaryMonitor(), NULL);
	else // windowed
		wnd = glfwCreateWindow(GameConfig::get().configResX, GameConfig::get().configResY, "ASUM PROJECT", NULL, NULL);

	glfwMakeContextCurrent(wnd);
	glewInit();

	Game game;
	game.init(wnd);
	glfwSwapInterval(1);
	game.mainLoop();

	glfwDestroyWindow(wnd);
	glfwTerminate();

#ifdef _DEBUG
	Debug::CloseDebugStream();
#endif

	return 0;
}