#include "../GraphGUI.h"
#include "Plot.h"
#include <iostream>
#include <stdint.h>
#include <string>

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 800

int ScatterPlot::show()
{
	int initialised;
	auto app = GraphGUI(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, shaders, initialised);
	if (initialised < 0)
		return -1;
	app.runProgramLoop(this);
	return 0;
}

void ScatterPlot::draw(GLFWwindow *window, int program, int screenWidth, int screenHeight)
{
	double posx, posy;
	glfwGetCursorPos(window, &posx, &posy);
	std::cout << posx << " " << posy << std::endl;

	glUniform2f(glGetUniformLocation(program, "xaxis.start"), 30, 30);
	glUniform2f(glGetUniformLocation(program, "xaxis.end"), screenHeight - 30, 30);
	glUniform1f(glGetUniformLocation(program, "xaxis.angle"), -420);
	glUniform2f(glGetUniformLocation(program, "yaxis.start"), 30, screenWidth - 30);
	glUniform2f(glGetUniformLocation(program, "yaxis.end"), 30, 30);
	glUniform1f(glGetUniformLocation(program, "yaxis.angle"), -420);

	this->x_range.set(INT_MAX, INT_MIN, 1);
	this->y_range.set(INT_MAX, INT_MIN, 1);
	for (auto point : this->data) {
		if (this->x_range.start > point.x)
			this->x_range.start = point.x;
		if (this->x_range.end < point.x)
			this->x_range.end = point.x;
		if (this->y_range.start > point.y)
			this->y_range.start = point.y;
		if (this->y_range.end < point.y)
			this->y_range.end = point.y;
	}
	double xscale = (screenWidth - 80) / (this->x_range.end - this->x_range.start);
	double yscale = (screenHeight - 80) / (this->y_range.end - this->y_range.start);
	glUniform1i(glGetUniformLocation(program, "numberPoints"), this->data.size());
	for (int i = 0; i < this->data.size(); i++) {
		// std::cout << this->data[i].x * xscale << " " << this->data[i].y * yscale << std::endl;
		glUniform1f(glGetUniformLocation(program, ("points[" + std::to_string(i) + "].x").c_str()), (this->data[i].x - this->x_range.start) * xscale + 40);
		glUniform1f(glGetUniformLocation(program, ("points[" + std::to_string(i) + "].y").c_str()), (this->data[i].y - this->y_range.start) * yscale + 40);
	}
	// std::cout << xscale << " " << yscale << std::endl;

	this->x_range.resetStride(10);
	this->y_range.resetStride(10);

	// Draw the whole thing.
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
}
