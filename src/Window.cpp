#include "Window.h"

Window* Window::CreateInstance(GLuint resolution_width, GLuint resolution_height)
{
    if( instance == nullptr )
    {
        return instance = new Window(resolution_width, resolution_height);
    }
    else
    return nullptr;
}