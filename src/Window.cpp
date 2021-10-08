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


void Window::Init()
{
	if (!glfwInit())
	{
		throw std::runtime_error("glfwInit failed");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfw_window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL scene", NULL, NULL);

	if (!glfw_window)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create window");
	}

	glfwMakeContextCurrent(glfw_window);

	glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK) //glewInit must be after a vaolid openGL context
	{
		throw std::runtime_error("glewInit failed");
	}
}