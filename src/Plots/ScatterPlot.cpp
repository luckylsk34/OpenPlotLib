#include "../GUIManager.h"
#include "../VertexBuffer.h"
#include "../shaders.h"
#include "Plot.h"
#include <boost/math/statistics/linear_regression.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <iostream>
#include <stdint.h>
#include <string>

namespace stats = boost::math::statistics;

#define DEFAULT_SCREEN_WIDTH 500
#define DEFAULT_SCREEN_HEIGHT 400
#define PI 3.1415926f
#define BUFFER_OFFSET(i) ((char *) NULL + (i))

float *create_point_vertices(GUIManager &app, std::vector<Point> &points)
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
	// Seperation of axis from the boundary of the window in pixels.
	int separation = 30;
	// Radius in pixels.
	int radiusPixel = 10;
	// Radius in pixels.
	// float radius = (float) radiusPixel / app.screenWidth;
	float x_radius = (float) radiusPixel / app.screenWidth;
	float y_radius = (float) radiusPixel / app.screenHeight;

	for (auto point : points | boost::adaptors::indexed(0)) {
		// transform the point
		point.value().x -= minx;
		point.value().y -= miny;
		point.value().x /= (maxx - minx);
		point.value().y /= (maxy - miny);
		point.value().x -= 0.5;
		point.value().y -= 0.5;
		point.value().x *= 1.5;
		point.value().y *= 1.5;

		point.value().x *= (float) (app.screenWidth - 2 * separation) / app.screenWidth;
		point.value().y *= (float) (app.screenHeight - 2 * separation) / app.screenHeight;

		// add the 6 vertices for the triangles.
		quad[30 * point.index() + 0] = point.value().x + x_radius;
		quad[30 * point.index() + 1] = point.value().y - y_radius;
		quad[30 * point.index() + 2] = 0;
		quad[30 * point.index() + 3] = 1;
		quad[30 * point.index() + 4] = -1;

		quad[30 * point.index() + 5] = point.value().x + x_radius;
		quad[30 * point.index() + 6] = point.value().y + y_radius;
		quad[30 * point.index() + 7] = 0;
		quad[30 * point.index() + 8] = 1;
		quad[30 * point.index() + 9] = 1;

		quad[30 * point.index() + 10] = point.value().x - x_radius;
		quad[30 * point.index() + 11] = point.value().y + y_radius;
		quad[30 * point.index() + 12] = 0;
		quad[30 * point.index() + 13] = -1;
		quad[30 * point.index() + 14] = 1;

		quad[30 * point.index() + 15] = point.value().x - x_radius;
		quad[30 * point.index() + 16] = point.value().y + y_radius;
		quad[30 * point.index() + 17] = 0;
		quad[30 * point.index() + 18] = -1;
		quad[30 * point.index() + 19] = 1;

		quad[30 * point.index() + 20] = point.value().x - x_radius;
		quad[30 * point.index() + 21] = point.value().y - y_radius;
		quad[30 * point.index() + 22] = 0;
		quad[30 * point.index() + 23] = -1;
		quad[30 * point.index() + 24] = -1;

		quad[30 * point.index() + 25] = point.value().x + x_radius;
		quad[30 * point.index() + 26] = point.value().y - y_radius;
		quad[30 * point.index() + 27] = 0;
		quad[30 * point.index() + 28] = 1;
		quad[30 * point.index() + 29] = -1;
	}
	return quad;
}

enum scatter_plot_programs {
	points_program,
	axis_program
};

int ScatterPlot::show()
{
	int initialised;
	auto app = GUIManager(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, shaders, initialised);
	if (initialised < 0)
		return -1;

	VertexBuffer vbo;
	float *quad = new float[this->data.size() * 30 + 8 + 4];
	auto pointquad = create_point_vertices(app, this->data);
	for (int i = 0; i < this->data.size() * 30; i++) {
		quad[i] = pointquad[i];
	}
	float axisquad[] = { -0.85, -0.85,
		                 0.85, -0.85,
		                 -0.85, -0.85,
		                 -0.85, 0.85 };
	for (int i = 0; i < 8; i++) {
		quad[this->data.size() * 30 + i] = axisquad[i];
	}

	auto x = std::vector<float>(this->data.size());
	auto y = std::vector<float>(this->data.size());
	for (auto point : this->data | boost::adaptors::indexed(0)) {
		x[point.index()] = point.value().x;
		y[point.index()] = point.value().y;
	}

	auto [c0, c1] = stats::simple_ordinary_least_squares(x, y);
	std::cout << "f(x) = " << c0 << " + " << c1 << "*x"
			  << "\n";
	std::cout << "f(10) = " << c0 + c1 * 10 << "\n";
	std::cout << "f(15.7) = " << c0 + c1 * 15.7 << "\n";

	quad[this->data.size() * 30 + 8] = this->data[0].x;
	quad[this->data.size() * 30 + 8 + 1] = c0 + c1 * this->data[0].x;
	quad[this->data.size() * 30 + 8 + 2] = this->data[this->data.size() - 1].x;
	quad[this->data.size() * 30 + 8 + 3] = c0 + c1 * this->data[this->data.size() - 1].x;

	vbo.send_data(quad, (this->data.size() * 30 + 8 + 4) * 4);

	app.create_program(points_program);
	app.bind_shaders(points_program, "scatter_plot_points_vertex", "scatter_plot_points_fragment");
	app.create_program(axis_program);
	app.bind_shaders(axis_program, "empty_vertex", "empty_fragment");

	glClear(GL_COLOR_BUFFER_BIT);
	while (!app.window_closed()) {
		glClearColor(1, 1, 1, 1);

		app.use_program(points_program);
		auto program = app.programs[points_program];
		int ATTRIB_VERTEX = glGetAttribLocation(program, "vertex");
		int ATTRIB_VALUE = glGetAttribLocation(program, "value");

		glEnableVertexAttribArray(ATTRIB_VERTEX);
		glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, 20, 0);
		glEnableVertexAttribArray(ATTRIB_VALUE);
		glVertexAttribPointer(ATTRIB_VALUE, 2, GL_FLOAT, GL_FALSE, 20, BUFFER_OFFSET(12));
		glDrawArrays(GL_TRIANGLES, 0, this->data.size() * 6);

		app.use_program(axis_program);
		program = app.programs[axis_program];
		ATTRIB_VERTEX = glGetAttribLocation(program, "vertex");

		glEnableVertexAttribArray(ATTRIB_VERTEX);
		glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, 8, BUFFER_OFFSET(this->data.size() * 30 * 4));
		// glEnable(GL_LINE_SMOOTH);
		glDrawArrays(GL_LINES, 0, 4);
		// glLineWidth(2);
		glDrawArrays(GL_LINES, 4, 2);
		// glDisable(GL_LINE_SMOOTH);

		app.post_draw_steps();
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
