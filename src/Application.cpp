#include "Plots/Plot.h"
#include <cmath>

#define DEFAULT_SCREEN_WIDTH 500
#define DEFAULT_SCREEN_HEIGHT 400

int main(int argc, char *argv[])
{
	po::options_description desc("Allowed options");
	desc.add_options()("help", "produce help message")("gl_draw_arrays", po::value<int>(), "set compression level");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// // double circleRadius = 60.0;
	std::vector points { Point<double>({ 10, 100 }),
		                 Point<double>({ 10.2, 300 }),
		                 Point<double>({ 10.6, 500 }),
		                 Point<double>({ 11, 700 }),
		                 Point<double>({ 11.4, 900 }),
		                 Point<double>({ 12.1, 1000 }),
		                 Point<double>({ 12.3, 1200 }),
		                 Point<double>({ 12.7, 1400 }),
		                 Point<double>({ 13.3, 1600 }),
		                 Point<double>({ 13.7, 1710 }),
		                 Point<double>({ 14, 1700 }),
		                 Point<double>({ 14.3, 1600 }),
		                 Point<double>({ 14.7, 1500 }),
		                 Point<double>({ 15.1, 1400 }),
		                 Point<double>({ 15.7, 1300 }) };
	// std::vector<Point> points { Point({ 10, 10 }),
	// 	                        Point({ 20, 20 }),
	// 	                        Point({ 30, 30 }),
	// 	                        Point({ 40, 40 }),
	// 	                        Point({ 50, 50 }) };
	std::vector<Point<double>> points2(360);
	for (int i = 0; i < points2.size();i++) {
		points2[i].x = i;
		points2[i].y = sin(i * 3.14 / 180);
	}

	// Plot *plot = new _2DPlot();

	ScatterPlotOptions options;
	options.set_resolution(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
	options.enable_ticks(false);
	options.set_tick_length(18);
	options.set_axes_seperation(30);
	options.set_point_radius(10);

	auto plot1 = std::make_unique<ScatterPlot>(points2, vm, options);
	plot1->show();

	return 0;
}
