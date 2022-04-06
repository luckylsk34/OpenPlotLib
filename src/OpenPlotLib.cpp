#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include <iostream>

#include "Plots/Plot.h"
#include "shaders.h"

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 800

namespace py = boost::python;

template <typename T>
inline std::vector<T> to_std_vector(const py::object &iterable)
{
	return std::vector<T>(py::stl_input_iterator<T>(iterable),
	                      py::stl_input_iterator<T>());
}

int emptyGraph(py::list x_data, py::list y_data)
{
	// double circleRadius = 60.0;
	// std::vector<Circle> circles { Circle({ 1000, 100 }, circleRadius),
	//	                          Circle({ 1000, 300 }, circleRadius),
	//	                          Circle({ 1000, 500 }, circleRadius),
	//	                          Circle({ 1000, 700 }, circleRadius),
	//	                          Circle({ 1000, 900 }, circleRadius),
	//	                          Circle({ 1200, 000 }, circleRadius),
	//	                          Circle({ 1200, 200 }, circleRadius),
	//	                          Circle({ 1200, 400 }, circleRadius),
	//	                          Circle({ 1200, 600 }, circleRadius),
	//	                          Circle({ 1200, 800 }, circleRadius),
	//	                          Circle({ 1400, 100 }, circleRadius),
	//	                          Circle({ 1400, 300 }, circleRadius),
	//	                          Circle({ 1400, 500 }, circleRadius),
	//	                          Circle({ 1400, 700 }, circleRadius),
	//	                          Circle({ 1400, 900 }, circleRadius) };
	// Ray ray = Ray({ 400, 450 }, 50);
	// Scene scene = Scene(circles, ray);

	std::vector<Point> points;
	// for (int i = 0; i < len(data); i++) {
	// 	points.push_back(Point(data[i][0], data[i][1]));
	// }
	auto x_ = to_std_vector<double>(x_data);
	auto y_ = to_std_vector<double>(y_data);
	for (int i = 0; i < x_.size(); i++) {
		points.push_back(Point(x_[i], y_[i]));
	}

	// auto points = to_std_vector<double>(points);
	Plot *plot1 = new ScatterPlot(points);
	plot1->show();
	return 0;
}

BOOST_PYTHON_MODULE(OpenPlotLib)
{
	using namespace boost::python;
	def("empty_graph", emptyGraph);
}
