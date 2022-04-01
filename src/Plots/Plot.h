#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "boost/program_options.hpp"
#include <iostream>
#include <utility>
#include <vector>

#include "../GL/GUIManager.h"
#include "options.h"

namespace po = boost::program_options;

template <typename T>
class Point
{
public:
	T x;
	T y;

	Point()
		: x(0)
		, y(0) {};
	Point(T x, T y)
		: x(x)
		, y(y) {};

	T length() { return (T) std::sqrt(pow(x, 2) + pow(y, 2)); }
	template <typename U>
	T dot(Point<U> const &right) { return x * right.x + y * right.y; }
	template <typename U>
	Point operator+(const Point<U> &right) { return Point(x + right.x, y + right.y); }
	template <typename U>
	Point operator-(const Point<U> &right) { return Point(x - right.x, y - right.y); }
	template <typename U>
	friend Point operator+(const Point &left, const U &right) { return Point(left.x + right, left.y + right); }
	template <typename U>
	friend Point operator+(const U &left, const Point &right) { return Point(left + right.x, left + right.y); }
	template <typename U>
	friend Point operator-(const Point &left, const U &right) { return Point(left.x - right, left.y - right); }
	template <typename U>
	friend Point operator-(const U &left, const Point &right) { return Point(left - right.x, left - right.y); }
	template <typename U>
	Point operator*(const Point<U> &scale) { return Point(x * scale.x, y * scale.y); }
	template <typename U>
	Point operator/(const Point<U> &scale) { return Point(x / scale.x, y / scale.y); }
	template <typename U>
	friend Point operator*(const Point &left, const U &right) { return Point(left.x * right, left.y * right); }
	template <typename U>
	friend Point operator*(const U &left, const Point &right) { return Point(left * right.x, left * right.y); }
	template <typename U>
	friend Point operator/(const Point &left, const U &right) { return Point(left.x / right, left.y / right); }
	template <typename U>
	friend Point operator/(const U &left, const Point &right) { return Point(left / right.x, left / right.y); }
	template <typename U>
	Point operator+=(const Point<U> &right)
	{
		x += right.x, y += right.y;
		return *this;
	}
	template <typename U>
	Point operator-=(const Point<U> &right)
	{
		x -= right.x, y -= right.y;
		return *this;
	}
	template <typename U>
	Point operator+=(const U &right)
	{
		x += right, y += right;
		return *this;
	}
	template <typename U>
	Point operator-=(const U &right)
	{
		x -= right, y -= right;
		return *this;
	}
	template <typename U>
	Point operator*=(const Point<U> &right)
	{
		x *= right.x, y *= right.y;
		return *this;
	}
	template <typename U>
	Point operator/=(const Point<U> &right)
	{
		x /= right.x, y /= right.y;
		return *this;
	}
	template <typename U>
	Point operator*=(const U &scale)
	{
		x *= scale, y *= scale;
		return *this;
	}
	template <typename U>
	Point operator/=(const U &scale)
	{
		x /= scale, y /= scale;
		return *this;
	}
	template <typename U>
	Point scale(U scale)
	{
		x *= scale, y *= scale;
		return *this;
	}
	template <typename U, typename V>
	Point scale(U scale_x, V scale_y)
	{
		x *= scale_x, y *= scale_y;
		return *this;
	}

	friend std::ostream &operator<<(std::ostream &os, Point const &point)
	{
		return os << "Point(" << point.x << ", " << point.y << ")";
	}

	template <typename U>
	Point<U> to_type() { return Point<U>(x, y); };
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
	std::vector<Point<float>> data;
	ScatterPlotOptions options;

	ScatterPlot() {};

	ScatterPlot(po::variables_map vm, ScatterPlotOptions options)
		: ScatterPlot(std::vector<Point<float>>(), vm, options) {};

	ScatterPlot(std::vector<Point<float>> data, po::variables_map vm, ScatterPlotOptions options)
		: data(data)
		, vm(vm)
		, options(options)
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
