#pragma once


#include"VertexBuffer.h"

class VertexBufferLayout;

//Vertex Array Object
class VertexArray
{
private:
	
	unsigned vao_id = 0;
public:
	VertexArray();
	~VertexArray();

	const VertexBuffer* vbo_ptr = nullptr;

	void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);

	//examples of hard-coded functions
	//void AddBuffer2c(const VertexBuffer& vbo, const VertexBufferLayout& layout); //positions + colors
	//void AddBuffer2t(const VertexBuffer& vbo, const VertexBufferLayout& layout); //positions + texture coords
	//void AddBuffer3v(const VertexBuffer& vbo, const VertexBufferLayout& layout); //positions + colors + texture coords

	void Bind() const;
	void Unbind() const;
};