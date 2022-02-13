#include "GraphGUI.h"
#include "Plots/plot.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#define SCREENSHOT_PNG_PATH "screenshot.png"

GraphGUI::GraphGUI(int screenWidth, int screenHeight, std::map<std::string, std::string> shaders, int &initialised)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->shaders = shaders;

	initialised = prepareProgramAndWindow();
}

GraphGUI::~GraphGUI()
{
	glDeleteProgram(this->program);
	glfwTerminate();
}

std::string GraphGUI::GetShadderFromSource(const std::string &filepath)
{
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss;
	while (getline(stream, line)) ss << line << "\n";
	return ss.str();
}

unsigned int GraphGUI::CompileShader(unsigned int type, const std::string &source)
{
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	/*Error handling*/
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		auto message = (char *) alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertexshader" : "fragmentshader") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

bool GraphGUI::CheckForError(unsigned int program, int whatToCheck)
{
	int result;
	glGetProgramiv(program, whatToCheck, &result);
	if (result) return false;

	int length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
	auto message = (char *) alloca(length * sizeof(char));
	glGetProgramInfoLog(program, length, &length, message);

	std::cout << "Failed to link" << std::endl;
	std::cout << message << std::endl;
	return true;
}

unsigned int GraphGUI::CreateShader(const std::string &vertexshader, const std::string &fragmentshader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexshader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentshader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	if (CheckForError(program, GL_LINK_STATUS)) return 0;
	glValidateProgram(program);
	if (CheckForError(program, GL_VALIDATE_STATUS)) return 0;
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void GraphGUI::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void) scancode;
	(void) action;
	(void) mods;

	if (action == GLFW_PRESS) {
		// if (key == GLFW_KEY_F5) {
		//     reload_render_conf("render.conf");
		//     reload_user_textures();
		//     r_reload(&global_renderer);
		// } else if (key == GLFW_KEY_F6) {
		//     printf("Saving the screenshot at %s\n", SCREENSHOT_PNG_PATH);
		//     int width, height;
		//     glfwGetWindowSize(window, &width, &height);
		//     void *pixels = malloc(4 * width * height);
		//     if (pixels == NULL) {
		//         fprintf(stderr, "ERROR: could not allocate memory for pixels to make a screenshot: %s\n",
		//                 strerror(errno));
		//         return;
		//     }
		//     glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		//     if (!stbi_write_png(SCREENSHOT_PNG_PATH, width, height, 4, pixels, width * 4)) {
		//         fprintf(stderr, "ERROR: could not save %s: %s\n", SCREENSHOT_PNG_PATH, strerror(errno));
		//     }
		//     free(pixels);
		// } else if (key == GLFW_KEY_SPACE) {
		//     pause = !pause;
		// } else if (key == GLFW_KEY_Q) {
		//     exit(1);
		// }

		// if (pause) {
		//     if (key == GLFW_KEY_LEFT) {
		//         time -= MANUAL_TIME_STEP;
		//     } else if (key == GLFW_KEY_RIGHT) {
		//         time += MANUAL_TIME_STEP;
		//     }
		// }
	}
}

void GraphGUI::runProgramLoop(Plot *plot)
{
	glClear(GL_COLOR_BUFFER_BIT);
	while (!glfwWindowShouldClose(this->window)) {
		plot->draw(this->window, this->program, this->screenWidth, this->screenHeight);
		glfwSwapBuffers(this->window);
		// glfwPollEvents();
		glfwWaitEvents();
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

int GraphGUI::prepareProgramAndWindow()
{
	if (!glfwInit())
		return -1;

	this->window = glfwCreateWindow(this->screenWidth, this->screenHeight, "Hello World", nullptr, nullptr);
	if (!this->window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(this->window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, this->keyCallback);

	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	std::string vertexShaderString = this->shaders["vertex"];
	std::string fragmentShaderString = this->shaders["fragment"];
	this->program = CreateShader(vertexShaderString, fragmentShaderString);
	glUseProgram(this->program);
	return 0;
}
