#include "../GUIManager.h"
#include "../VertexBuffer.h"
#include "../shaders.h"
#include "Plot.h"
#include <boost/math/statistics/linear_regression.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <iostream>
#include <stdint.h>
#include <string>
#include <tuple>

namespace stats = boost::math::statistics;

#define DEFAULT_SCREEN_WIDTH 500
#define DEFAULT_SCREEN_HEIGHT 400
#define PI 3.1415926f
#define BUFFER_OFFSET(i) ((char *) NULL + (i))

std::tuple<float*, float, float, float, float> create_point_vertices(GUIManager &app, std::vector<Point> &points)
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

	float minx = FLT_MAX, maxx = FLT_MIN, miny = FLT_MAX, maxy = FLT_MIN;
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

	for (auto [index, point] : points | boost::adaptors::indexed(0)) {
		// transform the point
		point -= Point(minx, miny);
		point.scale(1/(maxx - minx), 1/(maxy - miny));
		point -= Point(0.5, 0.5);
		point.scale(1.5);

		point.x *= (float) (app.screenWidth - 2 * separation) / app.screenWidth;
		point.y *= (float) (app.screenHeight - 2 * separation) / app.screenHeight;

		// add the 6 vertices for the triangles.
		quad[30 * index + 0] = point.x + x_radius;
		quad[30 * index + 1] = point.y - y_radius;
		quad[30 * index + 2] = 0;
		quad[30 * index + 3] = 1;
		quad[30 * index + 4] = -1;

		quad[30 * index + 5] = point.x + x_radius;
		quad[30 * index + 6] = point.y + y_radius;
		quad[30 * index + 7] = 0;
		quad[30 * index + 8] = 1;
		quad[30 * index + 9] = 1;

		quad[30 * index + 10] = point.x - x_radius;
		quad[30 * index + 11] = point.y + y_radius;
		quad[30 * index + 12] = 0;
		quad[30 * index + 13] = -1;
		quad[30 * index + 14] = 1;

		quad[30 * index + 15] = point.x - x_radius;
		quad[30 * index + 16] = point.y + y_radius;
		quad[30 * index + 17] = 0;
		quad[30 * index + 18] = -1;
		quad[30 * index + 19] = 1;

		quad[30 * index + 20] = point.x - x_radius;
		quad[30 * index + 21] = point.y - y_radius;
		quad[30 * index + 22] = 0;
		quad[30 * index + 23] = -1;
		quad[30 * index + 24] = -1;

		quad[30 * index + 25] = point.x + x_radius;
		quad[30 * index + 26] = point.y - y_radius;
		quad[30 * index + 27] = 0;
		quad[30 * index + 28] = 1;
		quad[30 * index + 29] = -1;
	}
	// return {quad, minx, miny, maxx, maxy};
	return {quad, minx, miny, maxx, maxy};
}

enum scatter_plot_programs {
	points_program,
	axis_program
};

int ScatterPlot::show()
{
	int initialised;
	this->guiManager = GUIManager::get_instance();
	this->guiManager->start_window(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, shaders, initialised);
	if (initialised < 0)
		return -1;

	VertexBuffer vbo;
	// Points
	float *quad = new float[this->data.size() * 30 + 8 + 4];
	auto [pointquad, minx, miny, maxx, maxy] = create_point_vertices(*this->guiManager, this->data);
	for (int i = 0; i < this->data.size() * 30; i++)
		quad[i] = pointquad[i];

	// Axes
	float axisquad[] = { -0.85f, -0.85f,
		                 0.85f, -0.85f,
		                 -0.85f, -0.85f,
		                 -0.85f, 0.85f };
	for (int i = 0; i < 8; i++)
		quad[this->data.size() * 30 + i] = axisquad[i];

	// Regressor Line
	auto x = std::vector<float>(this->data.size());
	auto y = std::vector<float>(this->data.size());
	for (auto [index, point] : this->data | boost::adaptors::indexed(0)) {
		x[index] = point.x;
		y[index] = point.y;
	}
	auto [c0, c1] = stats::simple_ordinary_least_squares(x, y);
	quad[this->data.size() * 30 + 8] = this->data[0].x;
	quad[this->data.size() * 30 + 8 + 1] = c0 + c1 * this->data[0].x;
	quad[this->data.size() * 30 + 8 + 2] = this->data[this->data.size() - 1].x;
	quad[this->data.size() * 30 + 8 + 3] = c0 + c1 * this->data[this->data.size() - 1].x;

	vbo.send_data(quad, (int) (this->data.size() * 30 + 8 + 4) * 4);

	this->guiManager->create_program(points_program);
	this->guiManager->bind_shaders(points_program, "scatter_plot_points_vertex", "scatter_plot_points_fragment");
	this->guiManager->create_program(axis_program);
	this->guiManager->bind_shaders(axis_program, "empty_vertex", "empty_fragment");

	glClear(GL_COLOR_BUFFER_BIT);
	while (!this->guiManager->window_closed()) {
		glClearColor(1, 1, 1, 1);
		this->draw();
		this->guiManager->post_draw_steps();
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
	this->draw_points();
	this->draw_axes();
	// Draw Legend
}

void ScatterPlot::draw_points()
{
	this->guiManager->use_program(points_program);
	int program = this->guiManager->programs[points_program];
	int ATTRIB_VERTEX = glGetAttribLocation(program, "vertex");
	int ATTRIB_VALUE = glGetAttribLocation(program, "value");

	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, 20, 0);
	glEnableVertexAttribArray(ATTRIB_VALUE);
	glVertexAttribPointer(ATTRIB_VALUE, 2, GL_FLOAT, GL_FALSE, 20, BUFFER_OFFSET(12));
	glDrawArrays(GL_TRIANGLES, 0, (int) this->data.size() * 6);
}

void ScatterPlot::draw_axes()
{
	this->guiManager->use_program(axis_program);
	int program = this->guiManager->programs[axis_program];
	int ATTRIB_VERTEX = glGetAttribLocation(program, "vertex");

	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, 8, BUFFER_OFFSET(this->data.size() * 30 * 4));
	// glEnable(GL_LINE_SMOOTH);
	glDrawArrays(GL_LINES, 0, 4);
	// glLineWidth(2);
	glDrawArrays(GL_LINES, 4, 2);
	// glDisable(GL_LINE_SMOOTH);
}

void ScatterPlot::draw_legend()
{
}
