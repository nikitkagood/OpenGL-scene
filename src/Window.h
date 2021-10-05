#pragma once

#include<stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window //singleton - only one object can be created
{
public:
	const GLuint WIDTH, HEIGHT;
	const unsigned opengl_version = 4; //latest; there is no need for any lower version 
	static Window* instance;

	//static Window* Instance(GLuint resolution_width, GLuint resolution_height)
	//{
	//	if (instance == nullptr)
	//	{
	//		instance 
	//	}
	//}

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

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_version);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_version);
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

