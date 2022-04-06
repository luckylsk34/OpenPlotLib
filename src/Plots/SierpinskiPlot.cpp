#include "../GL/GUIManager.h"
#include "../GL/VertexBuffer.h"
#include "../shaders.h"
#include "Plot.h"
#include "options.h"
#include <boost/range/adaptor/indexed.hpp>
#include <iostream>
#include <random>

#define BUFFER_OFFSET(i) ((char *) NULL + (i))

void add_point_vertices(ScatterPlotOptions &options, std::vector<Point<float>> points, std::vector<float> &q1)
{
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

Point<float> SierpinskiPlot::get_next_point()
{
	simulated_points++;
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, data.size() - 1); // distribution in range [0, 2]

	auto choice = dist6(rng);
	return starting_pos = (data[choice] + starting_pos) / 2;
}

std::vector<Point<float>> SierpinskiPlot::get_next_points(int num_points)
{
	std::vector<Point<float>> points;
	for (int i = 0; i < num_points; i++)
		points.push_back(get_next_point());
	return points;
}

int SierpinskiPlot::show()
{
	int initialised;
	this->guiManager = GUIManager::get_instance();
	this->guiManager->start_window(this->options.screen_width(), this->options.screen_height(), shaders, initialised);
	if (initialised < 0)
		return -1;

	// Points
	std::vector<float> q1;
	std::vector<Point<float>> points = this->data;
	points.push_back(this->starting_pos);
	add_point_vertices(this->options, points, q1);

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
		this->guiManager->post_draw_steps(Event::Poll);

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

void SierpinskiPlot::draw_points(int num_points)
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
