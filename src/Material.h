#pragma once

#include <glm/glm.hpp>
#include "Shader.h"

class Material
{
private:
    //glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
public:
    //float-value defined material, doesn't work for now
    Material(glm::vec3 diffuse, glm::vec3 specular, float shininess) : diffuse(diffuse), specular(specular), shininess(shininess)
    {}

    //2 texture maps (diffused and specular) material
    //TODO: it probably needs to contain some info about texture maps
    Material(float shininess) : diffuse(0), specular(0), shininess(shininess)
    {}

    void Use(Shader& shader)
    {
        //shader.SetUniform3f("material.ambient", ambient.r, ambient.g, ambient.b); //we don't really need material.ambient lighting - it's the same as scene ambient lighting
        //shader.SetUniform3f("material.diffuse", diffuse.r, diffuse.g, diffuse.b); //Sampler2D now (i.e texture)
        //shader.SetUniform3f("material.specular", specular.r, specular.g, specular.b); //Sampler2D now (i.e texture)
        shader.SetUniform1f("material.shininess", shininess);
    }
};


