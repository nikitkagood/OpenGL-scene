#pragma once

#include "Texture.h"
#include "Material.h"

class VertexData
{
private:

public:
	
};

class Object
{
private:
	//Texture&& texture;
	//Material&& texture;
	VertexData&& vertices;

public:
	Object(VertexData vertices) : vertices(std::move(vertices))
	{
		
	}
};