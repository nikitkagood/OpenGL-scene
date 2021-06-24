#pragma once

#include <glm/glm.hpp>
#include "Shader.h"

class Material
{
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
public:
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
    {}

    void Use(Shader& shader)
    {
        //set if they are vec3, if they are Sampler2D (i.e textures) - they are commented
        shader.SetUniform3f("material.ambient", ambient.r, ambient.g, ambient.b);
        //shader.SetUniform3f("material.diffuse", diffuse.r, diffuse.g, diffuse.b);
        //shader.SetUniform3f("material.specular", specular.r, specular.g, specular.b);
        shader.SetUniform1f("material.shininess", shininess);
    }
};

