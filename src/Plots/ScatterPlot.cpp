#include "../GUIManager.h"
#include "../shaders.h"
#include "Plot.h"
#include <boost/range/adaptor/indexed.hpp>
#include <iostream>
#include <stdint.h>
#include <string>

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 800
#define PI 3.1415926f
#define BUFFER_OFFSET(i) ((char *) NULL + (i))

void initialize(GUIManager &app, std::vector<Point> &points)
{

	// Use a Vertex Array Object
	// GLuint vao;
	// glGenVertexArrays(1, &vao);
	// glBindVertexArray(vao);

	float right = 0.5;
	float bottom = -0.5;
	float left = -0.5;
	// +4 is for the vertices of axis.
	float top = 0.5;
	float *quad = new float[points.size() * 30];

	int minx = INT_MAX, maxx = INT_MIN, miny = INT_MAX, maxy = INT_MIN;
	for (auto point : points) {
		if (minx > point.x)
			minx = point.x;
		if (maxx < point.x)
			maxx = point.x;
		if (miny > point.y)
			miny = point.y;
		if (maxy < point.y)
			maxy = point.y;
	}
	// std::cout << "minx: " << minx << " maxx: " << maxx << std::endl;
	// std::cout << "miny: " << miny << " maxy: " << maxy << std::endl;
	// Seperation of axis from the boundary of the window in pixels.
	int separation = 30;
	// Radius in pixels.
	int radiusPixel = 5;
	// Radius in pixels.
	// float radius = (float) radiusPixel / app.screenWidth;
	float radius = 0.01;
	for (auto point : points | boost::adaptors::indexed(0)) {
		// transform the point
		// point.x *= (float)(app.screenWidth - 2 * separation) / (maxx - minx);
		// point.y *= (float)(app.screenHeight - 2 * separation) / (maxy - miny);
		// point.x -= separation;
		// point.y -= separation;
		// std::cout << "point-" << point.index() << ": " << point.value().x << " " << point.value().y << std::endl;
		point.value().x -= minx;
		point.value().y -= miny;
		point.value().x /= (maxx - minx);
		point.value().y /= (maxy - miny);
		// std::cout << "point-" << point.index() << ": " << point.value().x << " " << point.value().y << std::endl;
		point.value().x -= 0.5;
		point.value().y -= 0.5;
		// std::cout << "point-" << point.index() << ": " << point.value().x << " " << point.value().y << std::endl;
		point.value().x *= (float) (app.screenWidth - 2 * separation) / app.screenWidth;
		point.value().y *= (float) (app.screenHeight - 2 * separation) / app.screenHeight;
		// std::cout << "point-" << point.index() << ": " << point.value().x << " " << point.value().y << std::endl;
		// add the 6 vertices for the triangles.
		quad[30 * point.index() + 0] = point.value().x + radius;
		quad[30 * point.index() + 1] = point.value().y - radius;
		quad[30 * point.index() + 2] = 0;
		quad[30 * point.index() + 3] = 1;
		quad[30 * point.index() + 4] = -1;

		quad[30 * point.index() + 5] = point.value().x + radius;
		quad[30 * point.index() + 6] = point.value().y + radius;
		quad[30 * point.index() + 7] = 0;
		quad[30 * point.index() + 8] = 1;
		quad[30 * point.index() + 9] = 1;

		quad[30 * point.index() + 10] = point.value().x - radius;
		quad[30 * point.index() + 11] = point.value().y + radius;
		quad[30 * point.index() + 12] = 0;
		quad[30 * point.index() + 13] = -1;
		quad[30 * point.index() + 14] = 1;

		quad[30 * point.index() + 15] = point.value().x - radius;
		quad[30 * point.index() + 16] = point.value().y + radius;
		quad[30 * point.index() + 17] = 0;
		quad[30 * point.index() + 18] = -1;
		quad[30 * point.index() + 19] = 1;

		quad[30 * point.index() + 20] = point.value().x - radius;
		quad[30 * point.index() + 21] = point.value().y - radius;
		quad[30 * point.index() + 22] = 0;
		quad[30 * point.index() + 23] = -1;
		quad[30 * point.index() + 24] = -1;

		quad[30 * point.index() + 25] = point.value().x + radius;
		quad[30 * point.index() + 26] = point.value().y - radius;
		quad[30 * point.index() + 27] = 0;
		quad[30 * point.index() + 28] = 1;
		quad[30 * point.index() + 29] = -1;
	}

	unsigned int glBuffer;
	glGenBuffers(1, &glBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, glBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size() * 30, quad, GL_STATIC_DRAW);

	int ATTRIB_VERTEX = glGetAttribLocation(app.program, "vertex");
	int ATTRIB_VALUE = glGetAttribLocation(app.program, "value");

	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, 20, 0);
	glEnableVertexAttribArray(ATTRIB_VALUE);
	glVertexAttribPointer(ATTRIB_VALUE, 2, GL_FLOAT, GL_FALSE, 20, BUFFER_OFFSET(12));
}

int ScatterPlot::show()
{
	int initialised;
	auto app = GUIManager(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, shaders, initialised);
	if (initialised < 0)
		return -1;

	initialize(app, this->data);

	glClear(GL_COLOR_BUFFER_BIT);
	while (!app.windowClosed()) {
		this->draw();
		app.postDrawSteps();
		glClear(GL_COLOR_BUFFER_BIT);
	}
	return 0;
}

void ScatterPlot::draw()
{
	// Set Background
	glClearColor(1, 1, 1, 1);
	// double posx, posy;
	// glfwGetCursorPos(window, &posx, &posy);
	// Draw Points
	glDrawArrays(GL_TRIANGLES, 0, this->data.size() * 6);
	// Draw Axis
	// Draw Legend
}
