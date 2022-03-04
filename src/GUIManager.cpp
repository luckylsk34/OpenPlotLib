#include "GUIManager.h"
// #include "Plots/Plot.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#define SCREENSHOT_PNG_PATH "screenshot.png"

GUIManager::GUIManager() { }

std::map<std::string, int> CompileShaders(std::map<std::string, std::string> shaders)
{
	std::map<std::string, int> compiledShaders;
	for (auto shader : shaders) {
		unsigned int type = (shader.first.find("vertex") != std::string::npos) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
		compiledShaders[shader.first] = GUIManager::CompileShader(type, shader.second);
	}
	return compiledShaders;
}

GUIManager::GUIManager(int screenWidth, int screenHeight, std::map<std::string, std::string> shaders, int &initialised)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	initialised = prepareProgramAndWindow(shaders);
	this->compiledShaders = CompileShaders(shaders);
}

GUIManager::~GUIManager()
{
	this->detachAllShaders();
	for (auto compiledShader : this->compiledShaders) {
		glDeleteShader(compiledShader.second);
	}
	glDeleteProgram(this->program);
	glfwTerminate();
}

std::string GUIManager::GetShadderFromSource(const std::string &filepath)
{
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss;
	while (getline(stream, line)) ss << line << "\n";
	return ss.str();
}

unsigned int GUIManager::CompileShader(unsigned int type, const std::string &source)
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

bool GUIManager::CheckForError(unsigned int program, int whatToCheck)
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

void GUIManager::detachAllShaders()
{
	int maxCount = 10;
	int count;
	GLuint *shaders = new GLuint[maxCount];
	glGetAttachedShaders(this->program, maxCount, &count, shaders);
	for (int i = 0; i < count; ++i)
		glDetachShader(this->program, shaders[i]);
	std::cout << "Detached " << count << " shaders" << std::endl;
	delete[] shaders;
}

unsigned int GUIManager::reBindShaders(std::string vertexShader, std::string fragmentShader)
{
	this->detachAllShaders();
	int compiledVertexShader = this->compiledShaders[vertexShader];
	int compiledFragmentShader = this->compiledShaders[fragmentShader];
	auto ret =  GUIManager::BindShaders(this->program, compiledVertexShader, compiledFragmentShader);
	glUseProgram(this->program);
	return ret;
}

unsigned int GUIManager::BindShaders(unsigned int program, const int vertexshader, const int fragmentshader)
{
	// std::cout << "Bind Shaders" << vertexshader << std::endl;
	glAttachShader(program, vertexshader);
	// std::cout << "Bind Shaders" << fragmentshader << std::endl;
	glAttachShader(program, fragmentshader);
	glLinkProgram(program);
	if (CheckForError(program, GL_LINK_STATUS)) return 0;
	glValidateProgram(program);
	if (CheckForError(program, GL_VALIDATE_STATUS)) return 0;
	return 1;
}

void GUIManager::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
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

bool GUIManager::windowClosed()
{
	return glfwWindowShouldClose(this->window);
}

void GUIManager::postDrawSteps()
{
	glfwSwapBuffers(this->window);
	// glfwPollEvents();
	glfwWaitEvents();
}

void GLAPIENTRY
MessageCallback(GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar *message,
                const void *userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
	        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
	        type, severity, message);
}


void empty(GLFWwindow *window, double xpos, double ypos) { }
int GUIManager::prepareProgramAndWindow(std::map<std::string, std::string> shaders)
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
	// glfwSetKeyCallback(window, this->keyCallback);
	// glfwSetCursorPosCallback(window, empty);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error" << std::endl;
		return -1;
	}

	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	// Enable alpha for colors.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->program = glCreateProgram();

	return 0;
}
