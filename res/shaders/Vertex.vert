#version 430 core

in vec4 position;

precision mediump float;

void main(void)
{
    // gl_Position = vec4(
    //     2 * (gl_VertexID / 2) - 1,
    //     2 * (gl_VertexID % 2) - 1,
    //     0.0,
    //     1.0);
    gl_Position = position;
    // gl_Position = gl_Vertex;
    
}
