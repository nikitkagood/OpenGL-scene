#pragma once

#include "Settings.h"

#include <stdint.h>

//VBOs are used to cut off multiple use of the same verticies in one object
//

//VBO example:
// 
//2 triangles -> square (that looks like rectangle though, unless proper transformations applied)
//array<unsigned, 6> indices =
//{
//    0, 1, 2,
//    2, 3, 0
//};
//IndexBuffer ibo(indices.data(), indices.size());

class VertexBuffer
{
private:
	const void* data_ptr;
	unsigned vbo_buffer_id = 0; //internal OpenGL ID
	unsigned size; //how many vertices
public:
	//NOTE: to get size of all elements in container you must do: container.size() * sizeof(Type)
	VertexBuffer(const void* data, uint64_t size_in_bytes);
#ifdef COPY_CONSTRUCTORS_ON
	VertexBuffer(const VertexBuffer& copy);
#else
	//copy constructor; many OpenGL objects are unique, thus copying is forbidden
	VertexBuffer(const VertexBuffer& copy) = delete;
#endif // COPY_CONSTRUCTOR_ON

	//move constructor
	VertexBuffer(VertexBuffer&& moved) noexcept;

	~VertexBuffer();

	void SetBufferData();

	void Bind() const;
	void Unbind() const;
	unsigned Size() const;

	void Clear();
};