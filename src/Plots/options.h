#pragma once

class ScatterPlotOptions
{
private:
	int _screen_width, _screen_height;
	double _tick_length;
	int _point_radius;
	int _axes_seperation;
	bool _ticks_enabled = false;

public:
	void set_resolution(int screen_width, int screen_height)
	{
		this->_screen_width = screen_width;
		this->_screen_height = screen_height;
	}

	void set_tick_length(double tick_length) { this->_tick_length = tick_length; }
	void set_point_radius(int point_radius) { this->_point_radius = point_radius; }
	void set_axes_seperation(int axes_seperation) { this->_axes_seperation = axes_seperation; }

	int screen_width() { return this->_screen_width; }
	int screen_height() { return this->_screen_height; }
	double tick_length() { return this->_tick_length; }
	int point_radius() { return this->_point_radius; }
	int axes_seperation() { return this->_axes_seperation; }

	void enable_ticks(bool enabled) { this->_ticks_enabled = enabled; }
	bool tick_enabled() { return this->_ticks_enabled; }
};

class SierpinskiPlotOptions : public ScatterPlotOptions
{
private:
	int num_simulations_per_frame = 1;

public:
	void set_num_simulations_per_frame(int num_simulations) { this->num_simulations_per_frame = num_simulations; }
	int get_num_simulations_per_frame() { return this->num_simulations_per_frame; }
};

class BarnsleyFernPlotOptions : public ScatterPlotOptions
{
private:
	int num_simulations_per_frame = 1;

public:
	void set_num_simulations_per_frame(int num_simulations) { this->num_simulations_per_frame = num_simulations; }
	int get_num_simulations_per_frame() { return this->num_simulations_per_frame; }
};