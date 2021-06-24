#pragma once

#include<stdexcept>

#include <GLFW/glfw3.h>
#include <GL/glew.h>

class Window
{
private:
	GLFWwindow* window = nullptr;

	void Init()
	{
		if (!glfwInit())
		{
			throw std::runtime_error("glfwInit failed");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _glfw_version);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _glfw_version);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL scene", NULL, NULL);

		if (!window)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create window");
		}

		glfwMakeContextCurrent(window);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (glewInit() != GLEW_OK) //glewInit must be after a vaolid openGL context
		{
			throw std::runtime_error("glewInit failed");
		}
	}

public:
	const GLuint WIDTH, HEIGHT;
	const unsigned _glfw_version;

	Window(GLuint resolution_width, GLuint resolution_height, unsigned glfw_version) : WIDTH(resolution_width), HEIGHT(resolution_height), _glfw_version(glfw_version)
	{
		if (glfw_version < 3)
		{
			throw std::invalid_argument("Minimal version is 3");
		}

		Init();
	}

	GLFWwindow* Get() const
	{
		return window;
	}

	void SetKeyCallback(GLFWkeyfun callback_func)
	{
		glfwSetKeyCallback(window, callback_func);
	}

	void SetCursorPosCallback(GLFWcursorposfun callback_func)
	{
		glfwSetCursorPosCallback(window, callback_func);
	}

	void SetScrollCallback(GLFWscrollfun callback_func)
	{
		glfwSetScrollCallback(window, callback_func);
	}
};

