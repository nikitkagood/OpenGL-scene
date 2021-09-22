#pragma once

#include"Settings.h"

#include"VertexBuffer.h"

class VertexBufferLayout;

//Vertex Array Object
class VertexArray
{
private:
	//const VertexBuffer* vbo_ptr = nullptr;
	unsigned vao_id = 0;
public:
	VertexArray();

#ifdef COPY_CONSTRUCTORS_ON
	VertexArray(const VertexArray& copy);
#else
	//copy constructor; many OpenGL objects are unique, thus copying is forbidden
	VertexArray(const VertexArray& copy) = delete;
#endif // COPY_CONSTRUCTORS_ON

	//move constructor
	VertexArray(VertexArray&& moved) noexcept;

	//using glDeleteVertexArrays(1, &vao_id) in destructor is undesirable because vector calls destructor on reallocation
	//because will invalidate OpenGL state: it will be thinking that there is no VAOs and it will be creating first VAO with id 1 all the time
	//use Clear instead
	~VertexArray(); 

	unsigned GetID() const;

	void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

	void Clear();
};