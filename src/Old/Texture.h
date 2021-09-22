//HANDLED BY ASSIMP NOW

//This class is used to manually load textures. Picture format like .bmp, .jpg etc.

/*
#pragma once

#include <SOIL/SOIL.h> 
#include <GL/glew.h>


#include <string> 


class Texture
{
private:
	int _width, _height;
	unsigned char* image_ptr = nullptr;
	unsigned texture_id;
	std::string _filepath;
public:
	Texture(std::string filepath) : _filepath(filepath)
	{
		image_ptr = SOIL_load_image(_filepath.c_str(), &_width, &_height, 0, SOIL_LOAD_RGB);
		if (image_ptr == nullptr)
		{
			std::cerr << "Texture" << filepath << "failed to load" << std::endl;
			SOIL_free_image_data(image_ptr);
			return;
		}
		//4th argument is number of color channels
		//5th argument is what information is needed to load (in this case we need RGB only)
		
		glGenTextures(1, &texture_id);
		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_ptr);)
		glGenerateMipmap(GL_TEXTURE_2D); //automatically generates mipmaps for the bound texture

		SOIL_free_image_data(image_ptr); //we created the texture and don't need the image, freeing memory
		Unbind();

	}

	void Bind()
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
	}

	//gl_texture_number - from 0 to 16; it's OpenGL specification - minimal support for 16 textures
	void Use(int gl_texture_number, unsigned shader_program, std::string shader_texture_name)
	{
		//shader_texture_name += std::to_string(gl_texture_number);
		
		glActiveTexture(GL_TEXTURE0 + gl_texture_number);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glUniform1i(glGetUniformLocation(shader_program, shader_texture_name.c_str()), gl_texture_number);
	}

	unsigned GetID()
	{
		return texture_id;
	}

	void Unbind() //unbind doesn't guarantee that exactly this object will be unbound!
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

//glTexImage2D:
//ѕервый аргумент описывает текстурную цель.”становив значение GL_TEXTURE_2D мы сообщили функции, что наша текстура прив€зана к этой цели(чтобы другие цели GL_TEXTURE_1D и GL_TEXTURE_3D не будут задействованы).
//¬торой аргумент описывает уровень мипмапа дл€ которого мы хотим сгенерировать текстуру, если вдруг мы хотим самосто€тельно сгенерировать мипмапы.ѕоскольку мы оставим генерацию мипмапов на OpenGL мы передадим 0.
//“ретий аргумент сообщает OpenGL в каком формате мы хотим хранить текстуру.ѕоскольку наше изображение имеет только RGB значени€ то и в текстуры мы также будем хранить только RGB значени€.
//„етвертый и п€тый аргументы задают ширину и высоту результирующей текстуры. ћы получили эти значени€ ранее во врем€ загрузки изображени€.
//Ўестой аргумент всегда должен быть 0. (јргумент устарел).
//—едьмой и восьмой аргументы описывают формат и тип данных исходного изображени€.ћы загружали RGB значени€ и хранили их в байтах(char) так что мы передаем эти значени€.
//ѕоследний аргумент Ч это сами данные изображени€.

*/