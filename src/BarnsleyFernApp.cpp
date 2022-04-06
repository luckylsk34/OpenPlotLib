#include "Plots/Plot.h"

#define DEFAULT_SCREEN_WIDTH 1000
#define DEFAULT_SCREEN_HEIGHT 800

int main(int argc, char *argv[])
{
	BarnsleyFernPlotOptions options;
	options.set_resolution(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
	options.enable_ticks(false);
	options.set_tick_length(18);
	options.set_axes_seperation(30);
	options.set_point_radius(1);
	options.set_num_simulations_per_frame(4000);

	auto plot1 = std::make_unique<BarnsleyFernPlot>(options);
	plot1->show();

	return 0;
}
