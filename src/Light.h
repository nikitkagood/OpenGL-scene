#pragma once

#include <glm/glm.hpp>
#include "Shader.h"

class Light
{
protected:
    //Phong lighting model; different components of lighting, their colors (RGB) and intensity (bigger/smaller value)
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    //lighting types, each vector means rgb
    //decrease all values respectively to decrease light intensity (i.e 0.2f, 0.2f, 0.2f - decreased intensity white color)
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : ambient(ambient), diffuse(diffuse), specular(specular)
    {}

    virtual void Use(Shader& shader) 
    {}
};

//it's Sun usually
class Light_Directional : protected Light
{
private:
    glm::vec3 direction; //example -0.2f, -1.0f, -0.3f is about 70 degrees
public:
    Light_Directional(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction) 
        : Light(ambient, diffuse, specular), direction(direction)
    {}

    void Use(Shader& shader) override
    {
        shader.SetUniform3f("light.ambient", ambient.r, ambient.g, ambient.b);
        shader.SetUniform3f("light.diffuse", diffuse.r, diffuse.g, diffuse.b);
        shader.SetUniform3f("light.specular", specular.r, specular.g, specular.b);

        shader.SetUniform3f("light.direction", direction.x, direction.y, direction.z);
    }
};

//a lamp for example
class Light_Dot : protected Light
{
private:
    //fade coefficients
    const GLfloat constant = 1.0f; //always 1
    GLfloat linear; //example 0.09f
    GLfloat quadratic; //example 0.032f
public:
    Light_Dot(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat linear_coef, GLfloat quadratic_coef) 
        : Light(ambient, diffuse, specular), linear(linear_coef), quadratic(quadratic_coef)
    {}

    void Use(Shader& shader) override
    {
        shader.SetUniform3f("light.ambient", ambient.r, ambient.g, ambient.b);
        shader.SetUniform3f("light.diffuse", diffuse.r, diffuse.g, diffuse.b);
        shader.SetUniform3f("light.specular", specular.r, specular.g, specular.b);

        shader.SetUniform1f("light.constant", constant);
        shader.SetUniform1f("light.linear", linear);
        shader.SetUniform1f("light.quadratic", quadratic);
    }
};

class Light_Spotlight : protected Light
{
private:
    //fade coefficients
    const GLfloat constant = 1.0f; //always 1
    GLfloat linear; //example 0.09f
    GLfloat quadratic; //example 0.032f

    glm::vec3 position;
    glm::vec3 direction;
    GLfloat cutOff; //cut off angle
    GLfloat outerCutOff; //outer cut off angle - to soften light edges; if this value equals cutOff - transition won't be soft at all
public:
    Light_Spotlight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat linear_coef, GLfloat quadratic_coef, glm::vec3 position, glm::vec3 direction, GLfloat cut_off_angle, GLfloat outer_cut_off)
        : Light(ambient, diffuse, specular), 
        linear(linear_coef), quadratic(quadratic_coef), position(position), direction(direction), cutOff(cut_off_angle), outerCutOff(outer_cut_off)
    {}

    void Use(Shader& shader) override
    {
        shader.SetUniform3f("light.ambient", ambient.r, ambient.g, ambient.b);
        shader.SetUniform3f("light.diffuse", diffuse.r, diffuse.g, diffuse.b);
        shader.SetUniform3f("light.specular", specular.r, specular.g, specular.b);

        shader.SetUniform1f("light.constant", constant);
        shader.SetUniform1f("light.linear", linear);
        shader.SetUniform1f("light.quadratic", quadratic);

        shader.SetUniform3f("light.position", position.x, position.y, position.z); //example camera.Position
        shader.SetUniform3f("light.direction", direction.x, direction.y, direction.z); //example camera.Front
        shader.SetUniform1f("light.cutOff", glm::cos(glm::radians(cutOff)));
        
        shader.SetUniform1f("light.outerCutOff", glm::cos(glm::radians(outerCutOff)));
    }
};