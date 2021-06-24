#pragma once

//also known as Element Buffer Object
class IndexBuffer
{
private:
	unsigned m_RendererID; //internal OpenGL ID
	unsigned m_Count; //ammount of indecies
public:
	IndexBuffer(const unsigned* data, unsigned count);

	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned GetCount() const { return m_Count; }
};