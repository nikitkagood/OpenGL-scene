//This class is used to manually load textures. Picture format like .bmp, .jpg etc.


#pragma once

#include <SOIL/SOIL.h> 
#include <GL/glew.h>

#include <iostream>
#include <string> 

enum class sm_TextureType //interchangeable with aiTextureType from assimp/Model.h
{
	DIFFUSE = 1,
	SPECULAR = 2,
};

class SimpleTexture
{
private:
	//int _width, _height;
	//unsigned char* image_ptr = nullptr;
	sm_TextureType type;
	unsigned texture_id;
	std::string _filepath;
public:
	SimpleTexture(const std::string& filepath, sm_TextureType type) : _filepath(filepath), type(type)
	{
		texture_id = textureFromFile(_filepath);
	}

	SimpleTexture(std::string&& filepath, sm_TextureType type) : _filepath(filepath), type(type)
	{
		texture_id = textureFromFile(_filepath);
	}

	~SimpleTexture() = default;

	unsigned int textureFromFile(const std::string& path)
	{
		unsigned int id = SOIL_load_OGL_texture(path.c_str(), 0, 0, SOIL_FLAG_INVERT_Y);

		if (!id)
		{
			std::cout << "ERROR::SOIL::Texture failed to load at path: " << path << std::endl;
			std::cout << SOIL_last_result() << std::endl;
		}

		return id;
	}

	//void textureFromFile_old(const std::string& path)
	//{
	//	image_ptr = SOIL_load_image(_filepath.c_str(), &_width, &_height, 0, SOIL_LOAD_RGB);
	//	if (image_ptr == nullptr)
	//	{
	//		std::cerr << "Texture" << path << "failed to load" << std::endl;
	//		SOIL_free_image_data(image_ptr);
	//		return ;
	//	}
	//	//4th argument is number of color channels
	//	//5th argument is what information is needed to load (in this case we need RGB only)

	//	glGenTextures(1, &texture_id);
	//	Bind();

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	// Set texture filtering
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_ptr));
	//	glGenerateMipmap(GL_TEXTURE_2D); //automatically generates mipmaps for the bound texture

	//	SOIL_free_image_data(image_ptr); //we created the texture and don't need the image, freeing memory
	//	Unbind();
	//}

	void Bind()
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
	}

	//gl_texture_number - from 0 to 16; it's OpenGL specification - minimal support for 16 textures
	void Use(int gl_texture_number, Shader& shader, std::string&& shader_texture_name)
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + gl_texture_number));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));
		//GLCall(glUniform1i(glGetUniformLocation(shader_program, shader_texture_name.c_str()), gl_texture_number));
		shader.SetUniform1i(shader_texture_name, gl_texture_number);
	}

	unsigned GetID()
	{
		return texture_id;
	}

	auto GetType()
	{
		return type;
	}

	void Unbind() //unbind doesn't guarantee that exactly this object will be unbound!
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

