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
        shader.SetUniform3f("directional_light.ambient", ambient.r, ambient.g, ambient.b);
        shader.SetUniform3f("directional_light.diffuse", diffuse.r, diffuse.g, diffuse.b);
        shader.SetUniform3f("directional_light.specular", specular.r, specular.g, specular.b);

        shader.SetUniform3f("directional_light.direction", direction.x, direction.y, direction.z);
    }
};

//all direction lamp for example
class Light_Point : protected Light
{
private:
    glm::vec3& position;

    //fade coefficients
    const GLfloat constant = 1.0f; //always 1
    GLfloat linear; //example 0.09f
    GLfloat quadratic; //example 0.032f
public:
    Light_Point(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat linear_coef, GLfloat quadratic_coef, glm::vec3& position)
        : Light(ambient, diffuse, specular), linear(linear_coef), quadratic(quadratic_coef), position(position)
    {}

    //void Use(Shader& shader) override
    //{
    //    shader.SetUniform3f("point_lights[0].position", light_position.x, light_position.y, light_position.z);

    //    shader.SetUniform3f("point_lights[0].ambient", ambient.r, ambient.g, ambient.b);
    //    shader.SetUniform3f("point_lights[0].diffuse", diffuse.r, diffuse.g, diffuse.b);
    //    shader.SetUniform3f("point_lights[0].specular", specular.r, specular.g, specular.b);

    //    shader.SetUniform1f("point_lights[0].constant", constant);
    //    shader.SetUniform1f("point_lights[0].linear", linear);
    //    shader.SetUniform1f("point_lights[0].quadratic", quadratic);
    //}

    void UseMany(Shader& shader, unsigned light_index)
    {
        std::string shader_struct_name = "point_lights[" + std::to_string(light_index) + "]";

        shader.SetUniform3f(shader_struct_name + ".position", position.x, position.y, position.z);

        shader.SetUniform3f(shader_struct_name + ".ambient", ambient.r, ambient.g, ambient.b);
        shader.SetUniform3f(shader_struct_name + ".diffuse", diffuse.r, diffuse.g, diffuse.b);
        shader.SetUniform3f(shader_struct_name + ".specular", specular.r, specular.g, specular.b);

        shader.SetUniform1f(shader_struct_name + ".constant", constant);
        shader.SetUniform1f(shader_struct_name + ".linear", linear);
        shader.SetUniform1f(shader_struct_name + ".quadratic", quadratic);
    }

};

class Light_Spotlight : protected Light
{
private:
    //fade coefficients
    const GLfloat constant = 1.0f; //always 1
    GLfloat linear; //example 0.09f
    GLfloat quadratic; //example 0.032f

    glm::vec3& position;
    glm::vec3& direction;
    GLfloat cutOff; //cut off angle
    GLfloat outerCutOff; //outer cut off angle - to soften light edges; if this value equals cutOff - transition won't be soft at all
public:
    Light_Spotlight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat linear_coef, GLfloat quadratic_coef, glm::vec3& position, glm::vec3& direction, GLfloat cut_off_angle, GLfloat outer_cut_off)
        : Light(ambient, diffuse, specular), 
        linear(linear_coef), quadratic(quadratic_coef), position(position), direction(direction), cutOff(cut_off_angle), outerCutOff(outer_cut_off)
    {}

    void Use(Shader& shader) override
    {
        shader.SetUniform3f("spot_light.position", position.x, position.y, position.z); //example camera.Position

        shader.SetUniform3f("spot_light.ambient", ambient.r, ambient.g, ambient.b);
        shader.SetUniform3f("spot_light.diffuse", diffuse.r, diffuse.g, diffuse.b);
        shader.SetUniform3f("spot_light.specular", specular.r, specular.g, specular.b);

        shader.SetUniform1f("spot_light.constant", constant);
        shader.SetUniform1f("spot_light.linear", linear);
        shader.SetUniform1f("spot_light.quadratic", quadratic);

        shader.SetUniform3f("spot_light.direction", direction.x, direction.y, direction.z); //example camera.Front
        shader.SetUniform1f("spot_light.cutOff", glm::cos(glm::radians(cutOff)));
        shader.SetUniform1f("spot_light.outerCutOff", glm::cos(glm::radians(outerCutOff)));
    }
};