#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include <iostream>

class Program
{
private:
	int _id;

public:
	Program() { _id = glCreateProgram(); }
	Program(const Program &program) { std::cout << "Copy Constructor" << std::endl;_id = program._id; }
	Program(Program &&program) { std::cout << "Move Constructor" << std::endl;_id = program._id; program._id = -1;}

	~Program()
	{
		std::cout << "Called destructor on Program(" << _id << ")" << std::endl;
		detach_all_shaders();
		glDeleteProgram(_id);
	};

	int id() { return _id; }
	void use() { glUseProgram(_id); }
	void detach_all_shaders();
	unsigned int bind_shaders(const Shader vertexshader, const Shader fragmentshader);
};