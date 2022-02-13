#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>
#include <memory>
#include <string>


#include "Plots/plot.h"

class GraphGUI
{
public:
	int screenWidth, screenHeight;
	std::map<std::string, std::string> shaders;
	GLFWwindow *window;
	unsigned int program;

	GraphGUI(int screenWidth, int screenHeight, std::map<std::string, std::string> shaders, int &initialised);
	~GraphGUI();
	void runProgramLoop(Plot *plot);

private:
	static std::string GetShadderFromSource(const std::string &filepath);
	static unsigned int CompileShader(unsigned int type, const std::string &source);
	static bool CheckForError(unsigned int program, int whatToCheck);
	static unsigned int CreateShader(const std::string &vertexshader, const std::string &fragmentshader);
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	int prepareProgramAndWindow();
};
