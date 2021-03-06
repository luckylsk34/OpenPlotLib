#include "Plots/Plot.h"

#define DEFAULT_SCREEN_WIDTH 1000
#define DEFAULT_SCREEN_HEIGHT 800

int main(int argc, char *argv[])
{
	po::options_description desc("Allowed options");
	desc.add_options()("help", "produce help message")("gl_draw_arrays", po::value<int>(), "set compression level");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// // double circleRadius = 60.0;
	std::vector points { Point<double>({ 10, 10 }),
		                 Point<double>({ 30, 10 }),
		                 Point<double>({ 20, 20 }) };

	SierpinskiPlotOptions options;
	options.set_resolution(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
	options.enable_ticks(false);
	options.set_tick_length(18);
	options.set_axes_seperation(30);
	options.set_point_radius(1);
	options.set_num_simulations_per_frame(4000);

	auto plot1 = std::make_unique<SierpinskiPlot>(
		points, Point<double>({ 15, 15 }), options);
	plot1->show();

	return 0;
}
