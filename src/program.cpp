#include "program.h"
#include <iostream>

void Program::detach_all_shaders()
{
	int maxCount = 10;
	int count;
	GLuint *shaders = new GLuint[maxCount];
	glGetAttachedShaders(_id, maxCount, &count, shaders);
	for (int i = 0; i < count; ++i)
		glDetachShader(_id, shaders[i]);
	delete[] shaders;
}

bool check_for_error(unsigned int program, int whatToCheck)
{
	int result;
	glGetProgramiv(program, whatToCheck, &result);
	if (result) return false;

	int length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
	auto message = (char *) alloca(length * sizeof(char));
	glGetProgramInfoLog(program, length, &length, message);

	std::cout << "Failed to link" << std::endl;
	std::cout << message << std::endl;
	return true;
}

unsigned int Program::bind_shaders(const Shader vertexshader, const Shader fragmentshader)
{
	std::cout << _id << std::endl;
	std::cout << vertexshader.id() << std::endl;
	glAttachShader(_id, vertexshader.id());
	std::cout << fragmentshader.id() << std::endl;
	glAttachShader(_id, fragmentshader.id());
	std::cout << "Linking Program" << std::endl;
	glLinkProgram(_id);
	if (check_for_error(_id, GL_LINK_STATUS)) return 0;
	glValidateProgram(_id);
	if (check_for_error(_id, GL_VALIDATE_STATUS)) return 0;
	return 1;
}
