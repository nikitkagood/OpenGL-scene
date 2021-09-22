// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include"IndexBuffer.h"
#include"Renderer.h"

IndexBuffer::IndexBuffer(const unsigned* data, uint64_t count) : m_Count(count), data(data)
{
    //ASSERT(sizeof(unsigned) == sizeof(GLuint)); //for debugging
#ifdef DEBUG_MODE_ON
    std::cerr << "IBO constructor" << std::endl;
#endif // DEBUG_MODE_ON

    Setup();
}

#ifdef COPY_CONSTRUCTORS_ON
IndexBuffer::IndexBuffer(const IndexBuffer& copy) : m_Count(copy.m_Count), data(copy.data), ibo_buffer_id(copy.ibo_buffer_id)
{
#ifdef DEBUG_MODE_ON
    std::cerr << "IBO copy constructor" << std::endl;
#endif // DEBUG_MODE_ON
}
#endif

IndexBuffer::IndexBuffer(IndexBuffer&& moved) noexcept : m_Count(std::move(moved.m_Count)), data(std::move(moved.data)), ibo_buffer_id(std::move(moved.ibo_buffer_id))
{
#ifdef DEBUG_MODE_ON
    std::cerr << "IBO move constructor" << std::endl;
#endif // DEBUG_MODE_ON
}

IndexBuffer::~IndexBuffer()
{
#ifdef DEBUG_MODE_ON
    std::cerr << "IBO destructor" << std::endl;
#endif // DEBUG_MODE_ON

    //Clear();
}

void IndexBuffer::Setup()
{
    GLCall(glGenBuffers(1, &ibo_buffer_id));
    Bind();
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned), data, GL_STATIC_DRAW));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_buffer_id));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::Clear()
{
    GLCall(glDeleteBuffers(1, &ibo_buffer_id));
}
