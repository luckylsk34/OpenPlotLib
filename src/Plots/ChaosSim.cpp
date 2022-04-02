#include "../GL/GUIManager.h"
#include "../GL/VertexBuffer.h"
#include "../shaders.h"
#include "Plot.h"
#include "options.h"
#include <boost/math/statistics/linear_regression.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <iostream>
#include <random>
#include <stdint.h>
#include <string>
#include <tuple>


namespace stats = boost::math::statistics;

#define PI 3.1415926f
#define BUFFER_OFFSET(i) ((char *) NULL + (i))

void add_point_vertices(ScatterPlotOptions &options, std::vector<Point<float>> points, std::vector<float> &q1)
{
	// Use a Vertex Array Object
	// GLuint vao;
	// glGenVertexArrays(1, &vao);
	// glBindVertexArray(vao);

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

	float x_radius = (float) options.point_radius() / options.screen_width();
	float y_radius = (float) options.point_radius() / options.screen_height();
	float *quad = new float[points.size() * 24];
	Point resolution = Point(options.screen_width(), options.screen_height());
	for (auto [index, point] : points | boost::adaptors::indexed(0)) {
		// transform the point
		point -= Point(minx, miny);
		point.scale(1 / (maxx - minx), 1 / (maxy - miny));
		point -= Point(0.5f, 0.5f);
		point.scale(1.5f);
		point *= 1 - 2.f * options.axes_seperation() / resolution;

		// add the 6 vertices for the triangles.
		int offset = 0;
		quad[24 * index + offset++] = point.x + x_radius;
		quad[24 * index + offset++] = point.y - y_radius;
		quad[24 * index + offset++] = 1;
		quad[24 * index + offset++] = -1;

		quad[24 * index + offset++] = point.x + x_radius;
		quad[24 * index + offset++] = point.y + y_radius;
		quad[24 * index + offset++] = 1;
		quad[24 * index + offset++] = 1;

		quad[24 * index + offset++] = point.x - x_radius;
		quad[24 * index + offset++] = point.y + y_radius;
		quad[24 * index + offset++] = -1;
		quad[24 * index + offset++] = 1;

		quad[24 * index + offset++] = point.x - x_radius;
		quad[24 * index + offset++] = point.y + y_radius;
		quad[24 * index + offset++] = -1;
		quad[24 * index + offset++] = 1;

		quad[24 * index + offset++] = point.x - x_radius;
		quad[24 * index + offset++] = point.y - y_radius;
		quad[24 * index + offset++] = -1;
		quad[24 * index + offset++] = -1;

		quad[24 * index + offset++] = point.x + x_radius;
		quad[24 * index + offset++] = point.y - y_radius;
		quad[24 * index + offset++] = 1;
		quad[24 * index + offset++] = -1;
	}
	q1.insert(q1.end(), quad, quad + points.size() * 24);
}

enum scatter_plot_programs {
	points_program,
	axis_program
};

template <typename T>
void add_regressor_points(std::vector<Point<float>> &points, std::vector<T> &vec)
{
	auto x = std::vector<float>(points.size());
	auto y = std::vector<float>(points.size());
	for (auto [index, point] : points | boost::adaptors::indexed(0)) {
		x[index] = point.x;
		y[index] = point.y;
	}
	auto [c0, c1] = stats::simple_ordinary_least_squares(x, y);
	vec.push_back(points.front().x);
	vec.push_back(c0 + c1 * points.front().x);
	vec.push_back(points.back().x);
	vec.push_back(c0 + c1 * points.back().x);
}

template <typename T>
void add_ticks(ScatterPlotOptions &options, std::vector<T> &vec)
{
	if (!options.tick_enabled())
		return;

	Point bottom_left(-0.75f, -0.75f), top_right(0.75f, 0.75f);
	Point resolution = Point(options.screen_width(), options.screen_height());
	bottom_left *= 1 - 2.f * options.axes_seperation() / resolution;
	top_right *= 1 - 2.f * options.axes_seperation() / resolution;

	int min_ruler_width = 40;
	auto num_ticks = (top_right - bottom_left) / 2 * resolution / min_ruler_width;
	auto tick_sep = (top_right - bottom_left) / (num_ticks - 1);
	auto start = bottom_left;

	for (int i = 0; i < num_ticks.x; i++) {
		vec.push_back(start.x + i * tick_sep.x);
		vec.push_back(-0.85f);
		vec.push_back(start.x + i * tick_sep.x);
		vec.push_back(-0.85f - options.tick_length() / options.screen_height());
	}
	for (int i = 0; i < num_ticks.y; i++) {
		vec.push_back(-0.85f);
		vec.push_back(start.y + i * tick_sep.y);
		vec.push_back(-0.85f - options.tick_length() / options.screen_width());
		vec.push_back(start.y + i * tick_sep.y);
	}
}

Point<float> ChaosSim::get_next_point()
{
	simulated_points++;
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, data.size() - 1); // distribution in range [0, 2]

	auto choice = dist6(rng);
	return starting_pos = (data[choice] + starting_pos) / 2;
}

std::vector<Point<float>> ChaosSim::get_next_points(int num_points)
{
	std::vector<Point<float>> points;
	for (int i = 0; i < num_points; i++)
		points.push_back(get_next_point());
	return points;
}

int ChaosSim::show()
{
	int initialised;
	this->guiManager = GUIManager::get_instance();
	this->guiManager->start_window(this->options.screen_width(), this->options.screen_height(), shaders, initialised);
	if (initialised < 0)
		return -1;
	unsigned int id = glCreateShader(GL_VERTEX_SHADER);
	id = glCreateShader(GL_FRAGMENT_SHADER);
	id = glCreateShader(GL_VERTEX_SHADER);
	id = glCreateShader(GL_FRAGMENT_SHADER);

	// Points
	std::vector<float> q1;
	std::vector<Point<float>> points = this->data;
	points.push_back(this->starting_pos);
	add_point_vertices(this->options, points, q1);

	// Axes
	// float axisquad[] = { -0.85f, -0.85f,
	// 	                 0.85f, -0.85f,
	// 	                 -0.85f, -0.85f,
	// 	                 -0.85f, 0.85f };
	// q1.insert(q1.end(), axisquad, axisquad + 8);
	// // Regressor Line
	// add_regressor_points(this->data, q1);

	// auto resolution = Point(this->guiManager->screen_width, this->guiManager->screen_height);
	// add_ticks(this->options, q1);

	VertexBuffer vbo(q1.data(), q1.size() * 4);

	this->guiManager->create_program(points_program);
	this->guiManager->bind_shaders(points_program, "scatter_plot_points_vertex", "scatter_plot_points_fragment");
	this->guiManager->create_program(axis_program);
	this->guiManager->bind_shaders(axis_program, "empty_vertex", "empty_fragment");

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	guiManager->swap_buffer();
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	while (!this->guiManager->window_closed()) {
		this->draw_points(points.size());
		this->guiManager->swap_buffer();
		this->draw_points(points.size());
		this->guiManager->post_draw_steps();

		points.clear();
		points.insert(points.end(), this->data.begin(), this->data.end());
		auto _points = this->get_next_points(this->options.get_num_simulations_per_frame());
		points.insert(points.end(), _points.begin(), _points.end());
		q1.clear();
		add_point_vertices(this->options, points, q1);
		vbo.send_data(q1.data(), q1.size() * 4);
		std::cout << "num_simulations: " << this->simulated_points << "\r";
	}
	return 0;
}

void ChaosSim::draw()
{
	// Set Background
	// glClearColor(1, 1, 1, 1);
	// double posx, posy;
	// glfwGetCursorPos(window, &posx, &posy);
	std::cout << "Drawing Points" << std::endl;
	this->draw_points(1);
	// std::cout << "Drawing Axes" << std::endl;
	// this->draw_axes();
	// Draw Legend
}

void ChaosSim::draw_points(int num_points)
{
	this->guiManager->use_program(points_program);
	Program program = this->guiManager->programs[points_program];
	int ATTRIB_VERTEX = glGetAttribLocation(program.id(), "vertex");
	int ATTRIB_VALUE = glGetAttribLocation(program.id(), "value");

	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, 16, 0);
	glEnableVertexAttribArray(ATTRIB_VALUE);
	glVertexAttribPointer(ATTRIB_VALUE, 2, GL_FLOAT, GL_FALSE, 16, BUFFER_OFFSET(8));
	glDrawArrays(GL_TRIANGLES, 0, (int) num_points * 6);
}

// void ChaosSim::draw_axes()
// {
// 	this->guiManager->use_program(axis_program);
// 	Program program = this->guiManager->programs[axis_program];
// 	int ATTRIB_VERTEX = glGetAttribLocation(program.id(), "vertex");

// 	glEnableVertexAttribArray(ATTRIB_VERTEX);
// 	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, 8, BUFFER_OFFSET(this->data.size() * 24 * 4));
// 	// glEnable(GL_LINE_SMOOTH);
// 	glDrawArrays(GL_LINES, 0, 4);
// 	// glLineWidth(2);
// 	glDrawArrays(GL_LINES, 4, 2);
// 	// glDisable(GL_LINE_SMOOTH);
// 	glDrawArrays(GL_LINES, 6, 32);
// 	glDrawArrays(GL_LINES, 6, 24);
// }

// void ChaosSim::draw_legend()
// {
// }
