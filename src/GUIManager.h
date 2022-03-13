#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include <map>
#include <memory>
#include <string>


class GUIManager
{
private:
	GUIManager();

public:
	int screen_width, screen_height;
	std::map<std::string, Shader> shaders;
	GLFWwindow *window;
	std::map<int, int> programs;
	static GUIManager *instance;

	GUIManager(const GUIManager &guiManager) = delete;
	GUIManager &operator=(const GUIManager &guiManager) = delete;

	void start_window(int screen_width, int screen_height, std::map<std::string, std::string> shaders, int &initialised);
	~GUIManager();
	static GUIManager *get_instance();
	bool window_closed();
	void post_draw_steps();
	static bool check_for_error(unsigned int program, int whatToCheck);
	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
	void create_program(const unsigned int program_name);
	void use_program(const unsigned int program_name);
	void detach_all_shaders(const unsigned int program_name);
	unsigned int rebind_shaders(const unsigned int program_name, std::string vertexShader, std::string fragmentShader);
	unsigned int bind_shaders(unsigned int program_name, const std::string vertexshader, const std::string fragmentshader);
	unsigned int bind_shaders(unsigned int program, const Shader vertexshader, const Shader fragmentshader);
	int prepare_program_and_window(std::map<std::string, std::string> shaders);
};
