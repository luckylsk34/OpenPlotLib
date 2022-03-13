#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "boost/program_options.hpp"
#include <iostream>
#include <utility>
#include <vector>

#include "../GUIManager.h"
#include "options.h"

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

	float length() { return (float) std::sqrt(pow(x, 2) + pow(y, 2)); }
	float dot(Point const &right) { return x * right.x + y * right.y; }
	Point operator+(const Point &right) { return Point(x + right.x, y + right.y); }
	Point operator-(const Point &right) { return Point(x - right.x, y - right.y); }
	friend Point operator+(const Point &left, const float &right) { return Point(left.x + right, left.y + right); }
	friend Point operator+(const float &left, const Point &right) { return Point(left + right.x, left + right.y); }
	friend Point operator-(const Point &left, const float &right) { return Point(left.x - right, left.y - right); }
	friend Point operator-(const float &left, const Point &right) { return Point(left - right.x, left - right.y); }
	Point operator*(const Point &scale) { return Point(x * scale.x, y * scale.y); }
	Point operator/(const Point &scale) { return Point(x / scale.x, y / scale.y); }
	friend Point operator*(const Point &left, const float &right) { return Point(left.x * right, left.y * right); }
	friend Point operator*(const float &left, const Point &right) { return Point(left * right.x, left * right.y); }
	friend Point operator/(const Point &left, const float &right) { return Point(left.x / right, left.y / right); }
	friend Point operator/(const float &left, const Point &right) { return Point(left / right.x, left / right.y); }
	Point operator+=(const Point &right) { x += right.x, y += right.y; return *this; }
	Point operator-=(const Point &right) { x -= right.x, y -= right.y; return *this; }
	Point operator+=(const float &right) { x += right, y += right; return *this; }
	Point operator-=(const float &right) { x -= right, y -= right; return *this; }
	Point operator*=(const Point &right) { x *= right.x, y *= right.y; return *this; }
	Point operator/=(const Point &right) { x /= right.x, y /= right.y; return *this; }
	Point operator*=(const float &scale) { x *= scale, y *= scale; return *this; }
	Point operator/=(const float &scale) { x /= scale, y /= scale; return *this; }
	Point scale(float scale) { x *= scale, y *= scale; return *this; }
	Point scale(float scale_x, float scale_y) { x *= scale_x, y *= scale_y; return *this; }
	
	friend std::ostream &operator<<(std::ostream &os, Point const &point) { 
    	return os << "Point(" << point.x << ", " << point.y << ")";
	}

	template <typename T>
	Point to_type() {return Point(T(x), T(y));};
	// operator int() const { return Point(int(x), int(y)); }
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
	GUIManager *guiManager;

	virtual ~Plot() = default;

	virtual int show() { return 0; };
	virtual void draw() {};
};

class _2DPlot : public Plot
{
public:
	Range<double> x_range;
	Range<double> y_range;

	int show() override;
	void draw() override;
};

class ScatterPlot : public _2DPlot
{
public:
	po::variables_map vm;
	std::vector<Point> data;
	ScatterPlotOptions options;

	ScatterPlot() {};

	ScatterPlot(po::variables_map vm,  ScatterPlotOptions options)
		: ScatterPlot(std::vector<Point>(), vm, options) {};

	ScatterPlot(std::vector<Point> data, po::variables_map vm, ScatterPlotOptions options)
		: data(data)
		, vm(vm)
		,options(options)
	{
		this->x_range.set(0, 100, 10);
		this->y_range.set(0, 100, 10);
	};

	int show() override;
	void draw() override;
	void draw_points();
	void draw_axes();
	void draw_legend();
};
