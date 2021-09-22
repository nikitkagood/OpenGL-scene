#pragma once


//UNUSED FOR NOW


#include "Renderer.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

//DEGREES ARE OFTENLY MUST BE CONVERTED INTO RADIANS BY glm::radians()

//ORDER OF TRANSFORMATIONS MATTERS A LOT
//there can be unexcpected results
//for example:
//if rotate then translate - object will rotate around his edge
//if translate, scale, rotate - object will be in a correct place but it will rotate other way
//sometimes resulting coordinates may be nulified so nothing will be displayed

void GLMTranslate(unsigned shaderID, float x, float y, float z) //x, y, z mean where to move
{
	if (x == 0 && y == 0 && z == 0)
	{
		std::cerr << "GLMTranslate: arguments are empty! Skipping..." << std::endl;
		return;
	}

	glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	transform = glm::translate(transform, glm::vec3(x, y, z)); //exammple: 0.5f, -0.5f, 0.0f

	GLCall(unsigned transformLoc = glGetUniformLocation(shaderID, "transform"));
	GLCall(glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform)));
}

void GLMRotate(unsigned shaderID, float x, float y, float z, float degrees) //x, y, z mean what axis to rotate
{
	if (x == 0 && y == 0 && z == 0)
	{
		std::cerr << "GLMRotate: arguments are empty! Skipping..." << std::endl;
		return;
	}

	glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	transform = glm::rotate(transform, degrees, glm::vec3(0.0f, 0.0f, 1.0f));

	// get matrix's uniform location and set matrix
	unsigned int transformLoc = glGetUniformLocation(shaderID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

}

void GLMScale(unsigned shaderID, int x_multiplier, int y_multiplier, int z_multiplier) //x, y, z mean what axis to scale
{
	if (x_multiplier == 0 && y_multiplier == 0 && z_multiplier == 0)
	{
		std::cerr << "GLMScale: arguments are empty! Skipping..." << std::endl;
		return;
	}

	glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	transform = glm::scale(transform, glm::vec3(x_multiplier, y_multiplier, z_multiplier)); //example 0.5, 0.5, 0.5

	GLCall(unsigned transformLoc = glGetUniformLocation(shaderID, "transform"));
	GLCall(glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform)));
}

void GLMRotate_over_time(unsigned shaderID, float x, float y, float z) //x, y, z mean what axis to rotate
{
	if (x == 0 && y == 0 && z == 0)
	{
		std::cerr << "GLMRotate_over_time: arguments are empty! Skipping..." << std::endl;
		return;
	}

	glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(x, y, z)); //example: 0.0f, 0.0f, 1.0f

	// get matrix's uniform location and set matrix
	unsigned int transformLoc = glGetUniformLocation(shaderID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

}




