// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include"VertexBuffer.h"
#include"Renderer.h"

VertexBuffer::VertexBuffer(const void* data, uint64_t size_in_bytes) : data_ptr(data), size(size_in_bytes)
{
#ifdef DEBUG_MODE_ON
    std::cerr << "VBO constructor" << std::endl;
#endif // DEBUG_MODE_ON

    GLCall(glGenBuffers(1, &vbo_buffer_id));
}

#ifdef COPY_CONSTRUCTORS_ON
VertexBuffer::VertexBuffer(const VertexBuffer& copy) : data_ptr(copy.data_ptr), vbo_buffer_id(copy.vbo_buffer_id), _size(copy.vbo_buffer_id)
{
#ifdef DEBUG_MODE_ON
    std::cerr << "VBO copy constructor" << std::endl;
#endif // DEBUG_MODE_ON
}
#endif

VertexBuffer::VertexBuffer(VertexBuffer&& moved) noexcept : data_ptr(std::move(moved.data_ptr)), vbo_buffer_id(std::move(moved.vbo_buffer_id)), size(std::move(moved.size))
{
#ifdef DEBUG_MODE_ON
    std::cerr << "VBO move constructor" << std::endl;
#endif // DEBUG_MODE_ON
}

VertexBuffer::~VertexBuffer()
{
#ifdef DEBUG_MODE_ON
    std::cerr << "VBO destructor" << std::endl;
#endif // DEBUG_MODE_ON

    //Clear();
}

void VertexBuffer::SetBufferData()
{
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data_ptr, GL_STATIC_DRAW));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo_buffer_id));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

unsigned VertexBuffer::Size() const
{
    return size;
}

void VertexBuffer::Clear()
{
    GLCall(glDeleteBuffers(1, &vbo_buffer_id));
}
