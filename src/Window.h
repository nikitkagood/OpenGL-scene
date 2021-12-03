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

	void SetCursorPosCallback(GLFWcursorposfun callback_func) const
	{
		glfwSetCursorPosCallback(glfw_window, callback_func);
	}

	void SetScrollCallback(GLFWscrollfun callback_func) const
	{
		glfwSetScrollCallback(glfw_window, callback_func);
	}

	~Window()
	{
		delete instance;
		instance = nullptr;
	}
protected:
	inline static Window* instance;

	Window(GLuint resolution_width, GLuint resolution_height) : WIDTH(resolution_width), HEIGHT(resolution_height)
	{
		Init();
	}
private:
	GLFWwindow* glfw_window = nullptr;

	void Init();

};


