// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include"Renderer.h"
#include<iostream>

bool keys[]; //forward declaration

using namespace std;

bool Renderer::wire_frame_mode = false;

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR) {}
}

//error handler
bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        cerr << "[OpenGL error] " << error << endl;
        cerr << "in function: " << function << " " << file << ":" << line << endl;
        return false;
    }
    return true;
}

void Renderer::GLClear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawElements(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) 
{
    shader.Bind();
    vao.Bind();
    ibo.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr)); //nullptr because we have nothing to bind
}

void Renderer::DrawArrays(const VertexArray& vao, const Shader& shader, unsigned vertices) 
{
    shader.Bind();
    vao.Bind();

    GLCall(glDrawArrays(GL_TRIANGLES, 0, vertices)); 

}

void Renderer::ToggleWireFrameMode()
{
    if (wire_frame_mode == false)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        wire_frame_mode = true;
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        wire_frame_mode = false;
    }
}

void Renderer::VSync(bool value)
{
    glfwSwapInterval((int)value);
}
