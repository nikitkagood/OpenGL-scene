#pragma once

#include<vector>
#include"Renderer.h"

//How to create a layout:
// VertexBufferLayout layout{ 3.0f, 3.0f, 2.0f };
//or:
// VertexBufferLayout layout;
// layout.Push<float>(3);
// layout.Push<float>(3);
// layout.Push<float>(2);


class VertexBufferLayout
{
private:
	struct VertexBufferElement
	{
		unsigned type;
		unsigned count;
		unsigned char normalized;
	};

	std::vector<VertexBufferElement> m_Elements;
	unsigned m_Stride = 0;
public:
	VertexBufferLayout() 
	{
#ifdef DEBUG_MODE_ON
		std::cerr << "VB Layout constructor" << std::endl;
#endif // DEBUG_MODE_ON
	}

	template<typename T>
	VertexBufferLayout(std::initializer_list<T> init_list) 
	{
#ifdef DEBUG_MODE_ON
		std::cerr << "VB Layout constructor(initializer_list)" << std::endl;
#endif // DEBUG_MODE_ON

		for(auto& i : init_list)
		{
			Push<T>(i);
		}
	}

#ifdef COPY_CONSTRUCTORS_ON
	VertexBufferLayout(const VertexBufferLayout& copy) : m_Elements(copy.m_Elements), m_Stride(copy.m_Stride)
	{
#ifdef DEBUG_MODE_ON
		std::cerr << "VB Layout copy constructor" << std::endl;
#endif // DEBUG_MODE_ON
	}
#else
	//copy constructor; many OpenGL objects are unique, thus copying is forbidden
	VertexBufferLayout(const VertexBufferLayout& copy) = delete;
#endif // COPY_CONSTRUCTORS_ON

	VertexBufferLayout(VertexBufferLayout&& moved) noexcept : m_Elements(std::move(moved.m_Elements)), m_Stride(std::move(moved.m_Stride))
	{
#ifdef DEBUG_MODE_ON
		std::cerr << "VB Layout move constructor" << std::endl;
#endif // DEBUG_MODE_ON
	}

	template<typename T>
	void Push(unsigned count)
	{
		static_assert(false); //push without template argument is forbidden
	}

	template<>
	void Push<float>(unsigned count)
	{
		m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
		m_Stride += sizeof(GLfloat) * count;
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