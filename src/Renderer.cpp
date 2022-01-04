// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include<iostream>

#include"Renderer.h"

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

//clearing buffers for each frame to display things correctly
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

void Renderer::SetBackgroundColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    glClearColor(red, green, blue, alpha);
}

void Renderer::SetBackgroundColor(ColorRGBA color)
{
    glClearColor(color.r, color.g, color.b, color.a);
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
    glfwSwapInterval(static_cast<int>(value));
}

void Renderer::SetBlending()
{
    //defaults are:
    //glBlendFunc(GL_ONE, GL_ZERO);
    //glBlendEquation(GL_FUNC_ADD);
    // - no blending, source (all color channels) * ONE + dest * ZERO = source, i.e. source overrides desitnation completely;
    //source is a new texture, destination - what exists already

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
}
