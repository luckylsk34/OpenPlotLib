#include "../GraphGUI.h"
#include "../shaders.h"
#include "Plot.h"
#include <iostream>
#include <stdint.h>
#include <string>

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 800
#define PI 3.1415926f

void initialize(GraphGUI &app)
{

	// Use a Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 4 triangles to be rendered
	GLfloat vertices_position[24] = {
		0.0,
		0.0,
		0.5,
		0.0,
		0.5,
		0.5,

		0.0,
		0.0,
		0.0,
		0.5,
		-0.5,
		0.5,

		0.0,
		0.0,
		-0.5,
		0.0,
		-0.5,
		-0.5,

		0.0,
		0.0,
		0.0,
		-0.5,
		0.5,
		-0.5,
	};

	// Create a Vector Buffer Object that will store the vertices on video memory
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Allocate space and upload the data from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position), vertices_position, GL_STATIC_DRAW);

	// Get the location of the attributes that enters in the vertex shader
	GLint position_attribute = glGetAttribLocation(app.program, "position");
	// Specify how the data for position can be accessed
	glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable the attribute
	glEnableVertexAttribArray(position_attribute);
}

int ScatterPlot::show()
{
	int initialised;
	auto app = GraphGUI(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, shaders, initialised);
	if (initialised < 0)
		return -1;

	initialize(app);

	app.runProgramLoop(this);
	return 0;
}

void DrawCircle(float cx, float cy, float r, int num_segments)
{
	// glBegin(GL_LINE_LOOP);
	// for(int ii = 0; ii < num_segments; ii++)
	// {
	//     float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle

	//     float x = r * cosf(theta);//calculate the x component
	//     float y = r * sinf(theta);//calculate the y component

	//     glVertex2f(x + cx, y + cy);//output vertex

	// }
	// glEnd();
	int i;
	int triangleAmount = num_segments; //# of triangles used to draw circle

	// GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(cx, cy); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			cx + (r * cos(i * twicePi / triangleAmount)),
			cy + (r * sin(i * twicePi / triangleAmount)));
	}
	glEnd();
}

void ScatterPlot::draw(GLFWwindow *window, int program, int screenWidth, int screenHeight)
{
	double posx, posy;
	glfwGetCursorPos(window, &posx, &posy);
	// std::cout << posx << " " << posy << "\r";

	// glBindVertexArray(vao);
	// std::cout << glGetError() << "          ";
	glDrawArrays(vm["gl_draw_arrays"].as<int>(), 0, 12);
	// std::cout << glGetError() << "          \r";

	// glBegin(GL_POINTS);

	// for (int i = 0; i < 10000; i++) {
	// 	glVertex2f(posx - 100, posy - 100);
	// }

	// glEnd();

	// DrawCircle(posx, posy, 50, 100);
	// Draw the whole thing.
	// glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
}

// void ScatterPlot::draw(GLFWwindow *window, int program, int screenWidth, int screenHeight)
// {
// 	double posx, posy;
// 	glfwGetCursorPos(window, &posx, &posy);

// 	glUniform2f(glGetUniformLocation(program, "xaxis.start"), 30, 30);
// 	glUniform2f(glGetUniformLocation(program, "xaxis.end"), screenHeight - 30, 30);
// 	glUniform1f(glGetUniformLocation(program, "xaxis.angle"), -420);
// 	glUniform2f(glGetUniformLocation(program, "yaxis.start"), 30, screenWidth - 30);
// 	glUniform2f(glGetUniformLocation(program, "yaxis.end"), 30, 30);
// 	glUniform1f(glGetUniformLocation(program, "yaxis.angle"), -420);

// 	this->x_range.set(INT_MAX, INT_MIN, 1);
// 	this->y_range.set(INT_MAX, INT_MIN, 1);
// 	for (auto point : this->data) {
// 		if (this->x_range.start > point.x)
// 			this->x_range.start = point.x;
// 		if (this->x_range.end < point.x)
// 			this->x_range.end = point.x;
// 		if (this->y_range.start > point.y)
// 			this->y_range.start = point.y;
// 		if (this->y_range.end < point.y)
// 			this->y_range.end = point.y;
// 	}
// 	double xscale = (screenWidth - 80) / (this->x_range.end - this->x_range.start);
// 	double yscale = (screenHeight - 80) / (this->y_range.end - this->y_range.start);
// 	glUniform1i(glGetUniformLocation(program, "numberPoints"), this->data.size());
// 	for (int i = 0; i < this->data.size(); i++) {
// 		glUniform1f(glGetUniformLocation(program, ("points[" + std::to_string(i) + "].x").c_str()), (this->data[i].x - this->x_range.start) * xscale + 40);
// 		glUniform1f(glGetUniformLocation(program, ("points[" + std::to_string(i) + "].y").c_str()), (this->data[i].y - this->y_range.start) * yscale + 40);
// 	}

// 	this->x_range.resetStride(10);
// 	this->y_range.resetStride(10);

// 	// Draw the whole thing.
// 	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
// }
