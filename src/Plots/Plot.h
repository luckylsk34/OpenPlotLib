#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <utility>
#include <vector>


#include "../GL/GUIManager.h"
#include "options.h"

namespace po = boost::program_options;
namespace bu = boost::numeric::ublas;

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

class SierpinskiPlot : public _2DPlot
{
public:
	std::vector<Point<float>> data;
	Point<float> starting_pos;
	SierpinskiPlotOptions options;
	int simulated_points = 0;

	SierpinskiPlot() {};

	SierpinskiPlot(SierpinskiPlotOptions options)
		: SierpinskiPlot(std::vector<Point<float>>(), Point<float>(0, 0), options) {};

	SierpinskiPlot(std::vector<Point<float>> data, Point<float> starting_pos, SierpinskiPlotOptions options)
		: data(data)
		, starting_pos(starting_pos)
		, options(options)
	{
		this->x_range.set(0, 100, 10);
		this->y_range.set(0, 100, 10);
	};

	int show() override;
	void draw_points(int num_points);
	Point<float> get_next_point();
	std::vector<Point<float>> get_next_points(int num_points);
};

class BarnsleyFernPlot : public _2DPlot
{
public:
	// Point<float> starting_pos { 0, 0 };
	bu::matrix<double> starting_pos { 2, 1 };
	std::vector<bu::matrix<double>> f;
	std::vector<bu::matrix<double>> f_;
	BarnsleyFernPlotOptions options;
	int simulated_points = 0;

	BarnsleyFernPlot() {};

	BarnsleyFernPlot(BarnsleyFernPlotOptions options)
		: options(options)
	{
		// this->x_range.set(0, 1, 10);
		// this->y_range.set(0, 1, 10);
		starting_pos(0, 0) = 0;
		starting_pos(1, 0) = 0;
		std::vector<float> _f1 { 0, 0, 0, 0.16 };
		std::vector<float> _f2 { 0.85, 0.04, -0.04, 0.85 };
		std::vector<float> _f3 { 0.20, -0.26, 0.23, 0.22 };
		std::vector<float> _f4 { -0.15, 0.28, 0.26, 0.24 };
		std::vector<float> _f1_ { 0.0, 0.0 };
		std::vector<float> _f2_ { 1.60, 0.85 };
		std::vector<float> _f3_ { 1.60, 0.07 };
		std::vector<float> _f4_ { 0.44, 0.07 };
		bu::matrix<double> f1 { 2, 2 };
		bu::matrix<double> f2 { 2, 2 };
		bu::matrix<double> f3 { 2, 2 };
		bu::matrix<double> f4 { 2, 2 };
		bu::matrix<double> f1_ { 2, 1 };
		bu::matrix<double> f2_ { 2, 1 };
		bu::matrix<double> f3_ { 2, 1 };
		bu::matrix<double> f4_ { 2, 1 };
		std::copy(_f1.begin(), _f1.end(), f1.data().begin());
		std::copy(_f2.begin(), _f2.end(), f2.data().begin());
		std::copy(_f3.begin(), _f3.end(), f3.data().begin());
		std::copy(_f4.begin(), _f4.end(), f4.data().begin());
		std::copy(_f1_.begin(), _f1_.end(), f1_.data().begin());
		std::copy(_f2_.begin(), _f2_.end(), f2_.data().begin());
		std::copy(_f3_.begin(), _f3_.end(), f3_.data().begin());
		std::copy(_f4_.begin(), _f4_.end(), f4_.data().begin());
		f.push_back(f1);
		f.push_back(f2);
		f.push_back(f3);
		f.push_back(f4);
		f_.push_back(f1_);
		f_.push_back(f2_);
		f_.push_back(f3_);
		f_.push_back(f4_);
	};

	int show() override;
	void draw_points(int num_points);
	Point<float> get_next_point();
	std::vector<Point<float>> get_next_points(int num_points);
};
