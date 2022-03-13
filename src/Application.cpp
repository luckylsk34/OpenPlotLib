#include "Plots/Plot.h"

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
	std::vector<Point> points { Point({ 10, 100 }),
		                        Point({ 10.2, 300 }),
		                        Point({ 10.6, 500 }),
		                        Point({ 11, 700 }),
		                        Point({ 11.4, 900 }),
		                        Point({ 12.1, 1000 }),
		                        Point({ 12.3, 1200 }),
		                        Point({ 12.7, 1400 }),
		                        Point({ 13.3, 1600 }),
		                        Point({ 13.7, 1710 }),
		                        Point({ 14, 1700 }),
		                        Point({ 14.3, 1600 }),
		                        Point({ 14.7, 1500 }),
		                        Point({ 15.1, 1400 }),
		                        Point({ 15.7, 1300 }) };
	// std::vector<Point> points { Point({ 10, 10 }),
	// 	                        Point({ 20, 20 }),
	// 	                        Point({ 30, 30 }),
	// 	                        Point({ 40, 40 }),
	// 	                        Point({ 50, 50 }) };

	// Plot *plot = new _2DPlot();

	ScatterPlotOptions options;
	options.set_resolution(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
	options.enable_ticks(false);
	options.set_tick_length(18);
	options.set_axes_seperation(30);
	options.set_point_radius(10);
	
	auto plot1 = std::make_unique<ScatterPlot>(points, vm, options);
	plot1->show();


	return 0;
}
