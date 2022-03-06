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
	std::map<int, int> programs;

	GUIManager();
	GUIManager(int screenWidth, int screenHeight, std::map<std::string, std::string> shaders, int &initialised);
	~GUIManager();
	bool window_closed();
	void post_draw_steps();
	static bool check_for_error(unsigned int program, int whatToCheck);
	static std::string get_shadder_from_source(const std::string &filepath);
	static unsigned int compile_shader(unsigned int type, const std::string &source);
	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
	void create_program(const unsigned int program_name);
	void use_program(const unsigned int program_name);
	void detach_all_shaders(const unsigned int program_name);
	unsigned int rebind_shaders(const unsigned int program_name, std::string vertexShader, std::string fragmentShader);
	unsigned int bind_shaders(unsigned int program_name, const std::string vertexshader, const std::string fragmentshader);
	unsigned int bind_shaders(unsigned int program, const int vertexshader, const int fragmentshader);
	int prepare_program_and_window(std::map<std::string, std::string> shaders);
};
