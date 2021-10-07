#pragma once

#include<stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window //singleton - only one object can be created
{
public:
	const GLuint WIDTH, HEIGHT; //for now they are intended to be unchangealbe
	const unsigned OPENGL_VERSION = 4; //latest; there is no need for any lower version 

	Window(const Window&) = delete;
	Window(Window&&) = delete;

	static Window* CreateInstance(GLuint resolution_width, GLuint resolution_height);

	GLFWwindow* Get() const
	{
		return glfw_window;
	}

	void SetKeyCallback(GLFWkeyfun callback_func)
	{
		glfwSetKeyCallback(glfw_window, callback_func);
	}

	void SetCursorPosCallback(GLFWcursorposfun callback_func)
	{
		glfwSetCursorPosCallback(glfw_window, callback_func);
	}

	void SetScrollCallback(GLFWscrollfun callback_func)
	{
		glfwSetScrollCallback(glfw_window, callback_func);
	}
protected:
	inline static Window* instance;

	Window(GLuint resolution_width, GLuint resolution_height) : WIDTH(resolution_width), HEIGHT(resolution_height)
	{
		Init();
	}
private:
	GLFWwindow* glfw_window = nullptr;

	void Init()
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

};


