#include "Plots/plot.h"

int main(void)
{
	// // float circleRadius = 60.0;
	std::vector<Point> points { Point({ 1000, 100 }),
								Point({ 1000, 300 }),
								Point({ 1000, 500 }),
								Point({ 1000, 700 }),
								Point({ 1000, 900 }),
								Point({ 1200, 000 }),
								Point({ 1200, 200 }),
								Point({ 1200, 400 }),
								Point({ 1200, 600 }),
								Point({ 1200, 800 }),
								Point({ 1400, 100 }),
								Point({ 1400, 300 }),
								Point({ 1400, 500 }),
								Point({ 1400, 700 }),
								Point({ 1400, 900 }) };
	// std::vector<Point> points { Point({ 10, 10 }),
	// 	                        Point({ 20, 20 }),
	// 	                        Point({ 30, 30 }),
	// 	                        Point({ 40, 40 }),
	// 	                        Point({ 50, 50 }) };
	// Ray ray = Ray({ 400, 450 }, 50);
	// Scene scene = Scene(circles, ray);

	// Plot *plot = new _2DPlot();

	auto plot1 = std::make_unique<ScatterPlot>(points);
	plot1->show();
	//_2DPlot plot;

	return 0;
}
