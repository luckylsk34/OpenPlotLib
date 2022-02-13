#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>
#include <iostream>

#include "GraphGUI.h"
#include "Plots/plot.h"
//#include "ReflectRNG/Scene.h"
#include "shaders.h"

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 800

int emptyGraph()
{
	// float circleRadius = 60.0;
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

	Plot *plot = new _2DPlot();
	//_2DPlot plot;

	int initialised;
	auto app = GraphGUI(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, shaders, initialised);
	if (initialised < 0)
		return -1;
	app.runProgramLoop(plot);
	return 0;
}

BOOST_PYTHON_MODULE(OpenPlotLib)
{
	using namespace boost::python;
	def("empty_graph", emptyGraph);
}
