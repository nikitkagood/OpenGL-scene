#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include"VertexArray.h"
#include"IndexBuffer.h"
#include"Shader.h"

//MSVC specific
#define ASSERT(x) if (!(x)) __debugbreak();

//error hanlder
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

//error handler
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
private:
    static bool wire_frame_mode;
public:
    void GLClear() const;

    void DrawElements(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
    void DrawArrays(const VertexArray& vao, const Shader& shader, unsigned vertices) const;

    static void ToggleWireFrameMode();
    static void VSync(bool value);
};