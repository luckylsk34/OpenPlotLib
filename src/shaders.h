#include <string>
#include <map>

static const std::map<std::string, std::string> shaders = {
    { "scatter_plot_points_fragment", R""""(#version 430 core

precision mediump float;
in vec2 val;
out vec4 out_color;

// #define LINE_THICCNESS 2
// #define LINE_LENGTH 2000
// #define POINT_RADIUS 5

void main() { 
	float R = 1.00; 
    // float R2 = 0.5; 
    // vec2 transformed = (gl_FragCoord.xy / 800) - 0.5;
    // float dist = sqrt(dot(transformed, transformed)) * 4;
    float dist = sqrt(dot(val, val));
    if (dist >= R) {
		discard;
	}
    float sm = smoothstep(R, R - 0.6, dist);
    float alpha = sm;
    out_color = vec4(gl_FragCoord.xy/800, 0.0, alpha);
}
)"""" },
    { "scatter_plot_points_vertex", R""""(#version 430 core

// in vec4 position;

precision mediump float;
in vec2 value;
in vec4 vertex;
out vec2 val;

// void main(void)
// {
//     // gl_Position = vec4(
//     //     2 * (gl_VertexID / 2) - 1,
//     //     2 * (gl_VertexID % 2) - 1,
//     //     0.0,
//     //     1.0);
//     gl_Position = position;
//     // gl_Position = gl_Vertex;
    
// }


// uniform mat4 viewMatrix; 
// uniform mat4 projectionMatrix;

void main() { 
    val = value;
    // gl_Position = projectionMatrix*viewMatrix*vertex; 
    gl_Position = vertex; 
}
)"""" },
};