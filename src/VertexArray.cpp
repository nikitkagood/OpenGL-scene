// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


#include"VertexArray.h"

#include"VertexBufferLayout.h"
#include"Renderer.h"

VertexArray::VertexArray()
{
#ifdef DEBUG_MODE_ON
	std::cerr << "VAO constructor" << std::endl;
#endif // DEBUG_MODE_ON

	GLCall(glGenVertexArrays(1, &vao_id));
}

#ifdef COPY_CONSTRUCTORS_ON
VertexArray::VertexArray(const VertexArray& copy) : vao_id(copy.vao_id)
{
#ifdef DEBUG_MODE_ON
	std::cerr << "VAO copy constructor" << std::endl;
#endif // DEBUG_MODE_ON

}
#endif

VertexArray::VertexArray(VertexArray&& moved) noexcept : vao_id(std::move(moved.vao_id))
{
#ifdef DEBUG_MODE_ON
	std::cerr << "VAO move constructor" << std::endl;
#endif // DEBUG_MODE_ON
}

VertexArray::~VertexArray()
{
#ifdef DEBUG_MODE_ON
	std::cerr << "VAO destructor" << std::endl;
#endif // DEBUG_MODE_ON

	//Clear();
}

unsigned VertexArray::GetID() const
{
	return vao_id;
}

//glVertexAttribPointer: id of argument in shader

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout) 
{
	Bind(); //bind buffer
	vbo.Bind();
	//vbo_ptr = vbo;

	//set up layout
	const auto& elements = layout.GetElements();
	unsigned offset = 0;
	unsigned stride = layout.GetStride();

	for (unsigned i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i)); //enable attribute
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (GLvoid*)offset)); //define attribute layout,
		offset += element.count * sizeof(float);
	}

	vbo.Unbind();
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(vao_id));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::Clear()
{
	GLCall(glDeleteVertexArrays(1, &vao_id));
}
