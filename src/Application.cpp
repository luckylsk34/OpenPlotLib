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

	// // float circleRadius = 60.0;
	std::vector points { Point<float>({ 10, 100 }),
		                 Point<float>({ 10.2f, 300 }),
		                 Point<float>({ 10.6f, 500 }),
		                 Point<float>({ 11, 700 }),
		                 Point<float>({ 11.4f, 900 }),
		                 Point<float>({ 12.1f, 1000 }),
		                 Point<float>({ 12.3f, 1200 }),
		                 Point<float>({ 12.7f, 1400 }),
		                 Point<float>({ 13.3f, 1600 }),
		                 Point<float>({ 13.7f, 1710 }),
		                 Point<float>({ 14, 1700 }),
		                 Point<float>({ 14.3f, 1600 }),
		                 Point<float>({ 14.7f, 1500 }),
		                 Point<float>({ 15.1f, 1400 }),
		                 Point<float>({ 15.7f, 1300 }) };
	// std::vector<Point> points { Point({ 10, 10 }),
	// 	                        Point({ 20, 20 }),
	// 	                        Point({ 30, 30 }),
	// 	                        Point({ 40, 40 }),
	// 	                        Point({ 50, 50 }) };
	std::vector<Point<float>> points2(360);
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
