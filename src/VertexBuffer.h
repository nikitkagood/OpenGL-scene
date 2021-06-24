#pragma once

class VertexBuffer
{
private:
	const void* data_ptr;
	unsigned vbo_id = 0; //internal OpenGL ID
	unsigned _size; //how many vertices
public:
	VertexBuffer(const void* data, unsigned size);
	~VertexBuffer();

	void SetBufferData();

	void Bind() const;
	void Unbind() const;
	unsigned Size() const;
};