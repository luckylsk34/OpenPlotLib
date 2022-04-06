
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
}

VertexBuffer::VertexBuffer(const void *data, size_t size)
{
	glGenBuffers(1, &buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	send_data(data, size);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &buffer_id);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::send_data(const void *data, size_t size) const
{
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
