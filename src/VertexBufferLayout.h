#pragma once

#include<vector>
#include"Renderer.h"

struct VertexBufferElement
{
	unsigned type;
	unsigned count;
	unsigned char normalized;

	//static unsigned GetSizeOfType(unsigned type) //I am not sure if this is needed
	//{
	//	switch (type)
	//	{
	//	case GL_FLOAT: return 4; //size of a type in bytes
	//	case GL_UNSIGNED_INT: return 4;
	//	case GL_UNSIGNED_BYTE: return 1;
	//	}
	//	ASSERT(false);

	//	return 0;
	//}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned m_Stride = 0;
public:
	VertexBufferLayout(){}

	//VertexBufferLayout(unsigned stride) : m_Stride(stride){}

	template<typename T>
	void Push(unsigned count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned count)
	{
		m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
		m_Stride += sizeof(GLfloat) * count;
		//m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT); //another way of doing it
	}

	template<>
	void Push<unsigned>(unsigned count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += sizeof(GLuint) * count;
	}

	template<>
	void Push<unsigned char>(unsigned count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += sizeof(GLubyte) * count;
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};