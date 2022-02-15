#pragma once

// #include "../shaders.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "boost/program_options.hpp"
#include <iostream>
#include <utility>
#include <vector>

namespace po = boost::program_options;

class Point
{
public:
	float x;
	float y;

	Point()
		: x(0)
		, y(0) {};
	Point(float x, float y)
		: x(x)
		, y(y) {};

	float length() { return std::sqrt(pow(x, 2) + pow(y, 2)); }
	float dot(Point const &right) { return x * right.x + y * right.y; }
	Point operator+(Point const &right) { return Point(x + right.x, y + right.y); }
	Point operator-(Point const &right) { return Point(x - right.x, y - right.y); }
};

template <typename T>
class Range
{
public:
	T start;
	T end;
	double stride;

	Range();
	Range(T start, T end, double stride)
		: start(start)
		, end(end)
		, stride(stride) {};

	void set(T start, T end, double stride)
	{
		this->start = start;
		this->end = end;
		this->stride = stride;
	}

	void resetStride(int numDivisions)
	{
		this->stride = (this->end - this->start) / numDivisions;
	}
};

template <typename T>
inline Range<T>::Range()
	: start(0)
	, end(0)
	, stride(0) {};

class Plot
{
public:
	virtual ~Plot() = default;

	virtual int show() { return 0; };
	virtual void draw(GLFWwindow *window, int program, int screenWidth, int screenHeight) {};
};

class _2DPlot : public Plot
{
public:
	Range<double> x_range;
	Range<double> y_range;

	// void draw(GLFWwindow *window, int program, int screenWidth, int screenHeight) override { std::cout << "insdi" << std::endl; };
	int show() override;
	void draw(GLFWwindow *window, int program, int screenWidth, int screenHeight) override;
};

class ScatterPlot : public _2DPlot
{
public:
	po::variables_map vm;
	std::vector<Point> data;

	ScatterPlot()
	{
		this->x_range.set(0, 100, 10);
		this->y_range.set(0, 100, 10);
	};

	ScatterPlot(std::vector<Point> data, po::variables_map vm)
		: data(data)
		, vm(vm)
	{
		this->x_range.set(0, 100, 10);
		this->y_range.set(0, 100, 10);
	};

	// void show();
	int show() override;
	void draw(GLFWwindow *window, int program, int screenWidth, int screenHeight) override;
};
