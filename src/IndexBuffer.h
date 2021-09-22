#pragma once

#include "Settings.h"

#include <stdint.h>

//also known as Element Buffer Object
class IndexBuffer
{
private:
	unsigned ibo_buffer_id; //internal OpenGL ID
	const unsigned* data;
	unsigned m_Count; //ammount of indecies
public:
	IndexBuffer(const unsigned* data, uint64_t count);

#ifdef COPY_CONSTRUCTORS_ON
	IndexBuffer(const IndexBuffer& copy);
#else
	//copy constructor
	IndexBuffer(const IndexBuffer& copy) = delete;
#endif // 

	//move constructor
	IndexBuffer(IndexBuffer&& moved) noexcept;

	//using glDeleteBuffers in destructor is undesirable because vector calls destructor on reallocation
	//use Clear instead (see VertexArray.h for further explanation)
	~IndexBuffer();

	void Setup();

	void Bind() const;
	void Unbind() const;

	inline unsigned GetCount() const { return m_Count; }

	void Clear();
};