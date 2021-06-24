// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


#include"VertexBuffer.h"
#include"Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned size) : data_ptr(data), _size(size)
{
    GLCall(glGenBuffers(1, &vbo_id));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &vbo_id));
}

void VertexBuffer::SetBufferData()
{
    GLCall(glBufferData(GL_ARRAY_BUFFER, _size, data_ptr, GL_STATIC_DRAW));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo_id));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

unsigned VertexBuffer::Size() const
{
    return _size;
}
