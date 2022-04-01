#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

class Shader
{
private:
	int _type;
	unsigned int _id;

public:
	Shader() {};
	Shader(unsigned int type)
		: _type(type) {};

	Shader(unsigned int type, const std::string &source)
		: _type(type)
	{
		_id = Shader::compile_shader(type, source);
	};

	void destroy() { glDeleteShader(_id); }

	unsigned int id() const { return _id; }

	static std::string get_shadder_from_source(const std::string &filepath);
	static unsigned int compile_shader(unsigned int type, const std::string &source);

	void compile_shader(const std::string &source) { _id = Shader::compile_shader(_type, source); };
};