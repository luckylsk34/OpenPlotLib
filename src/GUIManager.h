#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>
#include <memory>
#include <string>

class GUIManager
{
public:
	int screenWidth, screenHeight;
	std::map<std::string, int> compiledShaders;
	GLFWwindow *window;
	unsigned int program;

	GUIManager();
	GUIManager(int screenWidth, int screenHeight, std::map<std::string, std::string> shaders, int &initialised);
	~GUIManager();
	bool windowClosed();
	void postDrawSteps();
	static std::string GetShadderFromSource(const std::string &filepath);
	static unsigned int CompileShader(unsigned int type, const std::string &source);
	static bool CheckForError(unsigned int program, int whatToCheck);
	unsigned int BindShaders(std::string vertexShader, std::string fragmentShader);
	static unsigned int BindShaders(unsigned int program, const int vertexshader, const int fragmentshader);
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	int prepareProgramAndWindow(std::map<std::string, std::string> shaders);
};
