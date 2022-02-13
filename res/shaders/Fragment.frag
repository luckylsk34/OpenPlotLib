#version 430 core

precision mediump float;
out vec4 out_color;

#define LINE_THICCNESS 2
#define LINE_LENGTH 2000
#define POINT_RADIUS 5

struct Point
{
	float x;
	float y;
};
uniform int numberPoints;
uniform Point points[100];

struct Circle
{
	float x;
	float y;
	float radius;
};
uniform int numberCircles;
uniform Circle circles[100];

// Set Angle less than -360 to consider end point instead of angle.
struct Line
{
	vec2 start;
	vec2 end;
	float angle;
};
uniform int numberLines;
uniform Line lines[100];

void drawCircle(float x, float y, float radius)
{
	// if (pow(gl_FragCoord.xy.x - x, 2) + pow(gl_FragCoord.xy.y - y, 2) <= pow(radius + 1, 2))
	// 	out_color = vec4(0.682, 0.765, 0.918, 1.0);
	if (pow(gl_FragCoord.xy.x - x, 2) + pow(gl_FragCoord.xy.y - y, 2) <= pow(radius, 2))
		out_color = vec4(0.282, 0.471, 0.816, 1.0);
}

void drawLine(Line line)
{
	float x = gl_FragCoord.x;
	float y = gl_FragCoord.y;
	float x2 = line.start.x + LINE_LENGTH * cos(radians(line.angle));
	float y2 = line.start.y + LINE_LENGTH * sin(radians(line.angle));
	vec2 p1 = line.start;
	vec2 p2 = (line.angle < -360) ? line.end: vec2(x2, y2);
	vec2 p3 = {x, y};

	vec2 p12 = p2 - p1;
	vec2 p13 = p3 - p1;
	float d = dot(p12, p13) / length(p12);
	vec2 p14 = d * normalize(p12);
	vec2 p24 = p14 + p1 - p2;
	if (length(p13 - p14) <= LINE_THICCNESS && length(p14) <= length(p12) && length(p24) <= length(p12))
		out_color = vec4(0.0, 0.0, 0.0, 1.0);
	// out_color = vec4(0.0, 0.0, 1.0, 1.0);
}



struct Range
{
	float start;
	float end;
	float stride;
};
uniform Line xaxis;
uniform Line yaxis;
// uniform int windowWidth;
// uniform int windowHeight;

// void drawAxis(Range range)
// {
	
// }

void main(void)
{
	// Set the background color.
	out_color = vec4(1.0, 1.0, 1.0, 1.0);

	for (int i = 0; i < numberPoints; i++) {
		drawCircle(points[i].x, points[i].y, POINT_RADIUS);
	}

	// for (int i = 0; i < numberLines; i++)
	// 	drawLine(lines[i]);


	// Draw Axis
	// drawAxis();
	drawLine(xaxis);
	drawLine(yaxis);

	// PLot Points
	// PlotPoints();
}
