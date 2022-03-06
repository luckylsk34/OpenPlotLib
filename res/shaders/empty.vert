#version 430 core

precision mediump float;
in vec3 vertex;

void main() { 
    gl_Position = vec4(vertex, 1);
}
