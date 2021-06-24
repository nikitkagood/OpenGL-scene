// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


#include"VertexArray.h"

#include"VertexBufferLayout.h"
#include"Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &vao_id));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &vao_id));
}

//glVertexAttribPointer: id of argument in shader

//used when only 2d positions, but theoretically it should be used for all cases :/
void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout) 
{
	Bind(); //bind buffer
	vbo.Bind();
	vbo_ptr = &vbo;

	//set up layout
	const auto& elements = layout.GetElements();
	unsigned offset = 0;
	unsigned stride = layout.GetStride();

	
	for (unsigned i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i); //enable attribute
		glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (GLvoid*)offset); //define attribute layout,
		offset += element.count * sizeof(float);
	}

	vbo.Unbind();
}

//examples of hard-coded functions
//void VertexArray::AddBuffer2c(const VertexBuffer& vbo, const VertexBufferLayout& layout)
//{
//	Bind(); //bind buffer
//	vbo.Bind();
//
//	unsigned offset = 0;
//	unsigned stride = 6 * sizeof(GLfloat); //(3 + 3) * sizeof type
//	//Positions attribute
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
//	glEnableVertexAttribArray(0);
//
//	//Color attribute
//	offset += 3 * sizeof(GLfloat); //3 position coordinates * type
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
//	glEnableVertexAttribArray(1);
//
//
//}
//
//void VertexArray::AddBuffer2t(const VertexBuffer& vbo, const VertexBufferLayout& layout)
//{
//	Bind(); //bind buffer
//	vbo.Bind();
//
//	unsigned offset = 0;
//	unsigned stride = 5 * sizeof(GLfloat); //(2 + 3) * sizeof type
//	//Positions attribute
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
//	glEnableVertexAttribArray(0);
//
//	//Texture
//	offset += 3 * sizeof(GLfloat); //3 position coordinates * type
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
//	glEnableVertexAttribArray(2);
//}
//
//void VertexArray::AddBuffer3v(const VertexBuffer& vb, const VertexBufferLayout& layout)
//{
//	Bind(); //bind buffer
//	vb.Bind();
//
//	unsigned offset = 0;
//	unsigned stride = 8 * sizeof(GLfloat); //(3 + 3 + 2) * sizeof type
//	//Positions attribute
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
//	glEnableVertexAttribArray(0);
//
//	//Color attribute
//	offset += 3 * sizeof(GLfloat); //3 position coordinates * type
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
//	glEnableVertexAttribArray(1);
//
//	//Texture
//	offset += 3 * sizeof(GLfloat); ///3 color coordinates * type
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
//	glEnableVertexAttribArray(2);
//}


void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(vao_id));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
