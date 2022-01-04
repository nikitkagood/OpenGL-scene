#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include"Color.h"
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
public:
    static void GLClear();

    static void DrawElements(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader);
    static void DrawArrays(const VertexArray& vao, const Shader& shader, unsigned vertices);

    static void SetBackgroundColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    static void SetBackgroundColor(ColorRGBA color);

    static void ToggleWireFrameMode();
    static void VSync(bool value);

    static void SetBlending();

private:
    Renderer() = delete;

    static bool wire_frame_mode;
};