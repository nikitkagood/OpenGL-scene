#include "Settings.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <map>
#include <functional>

#include "Renderer.h"
#include "Window.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Material.h"
#include "Shader.h"
#include "Light.h"
#include "Model.h" //icludes: Assimp, SOIL
#include "SimpleModel.h" 

#include "Profiler.h"

//TODO LIST:
// FIX THE LIGHTING
//Model.cpp:
// processNode - correct child-parent relations
// 
//VAO, VBO etc - check for excessive of binds/unbinds
//Further development of color model
//Integrate std::filesystem?
//Separate Timer class?
// timer async?
//Update keyboard input - current system might not be fully correct
// bug: multimedia keys cause application to exit
// 
//Draw pipeline. Currently it is in a strict order: prerarations, then draw. Setting mat4 model results in everything that uses the same shader transforming to the last model.
//Everything that is being drawin with a lighting shader requires to have at least 1 directional light - otherwise it will be completely black
//glBindTextureUnit(0, texture0); //new way of binding textures
//GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
//
//Check textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end()) in Model::processMesh



const GLuint WIDTH = 1280, HEIGHT = 720;
bool keys[1024]; //contains statuses if pressed for all the keys; used to implement multiple keys input 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

Window* window = Window::CreateInstance(WIDTH, HEIGHT);
Camera camera1(*window, keys);

int main()
{
#ifdef BENCHMARK_MODE_ON
    LOG_DURATION("BENCHMARK: main, 1 game loop iteration");
#endif // BENCHMARK_MODE_ON

    using namespace std;

    //set OpenGL options
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    //blending
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    //Set the required callback functions
    window->SetKeyCallback(key_callback);

    //Define the viewport dimensions
    int vieport_width, vieport_height;
    glfwGetFramebufferSize(window->Get(), &vieport_width, &vieport_height);
    glViewport(0, 0, vieport_width, vieport_height);

    cout << glGetString(GL_VERSION) << endl;
        
    //another scope to make glfwTerminate work correclty and not throwing an error when GL window is closed
    {
        Model model_backpack("Models/backpack/backpack.obj"); 
        //Model model1("Models/table/table.obj"); //must be scaled down at least to 0.05f

        SimpleModel sm_WhiteCube;
        sm_WhiteCube.position = { 0.7f, 0.3f, 3.0f }; //y, z, x

        SimpleModel sm_Box;
        sm_Box.position = { 1.6f, 0.0f, -2.0f };
        //SimpleTexture tx_container2 ("Textures/container2.png", sm_TextureType::DIFFUSE);
        //SimpleTexture tx_container2_specular ("Textures/container2_specular.png", sm_TextureType::SPECULAR);
        sm_Box.addTexture(SimpleTexture("Textures/container2.png", sm_TextureType::DIFFUSE));
        sm_Box.addTexture(SimpleTexture("Textures/container2_specular.png", sm_TextureType::SPECULAR));
        sm_Box.addMaterial(Material{ 32.0f });

        Light_Directional light_directional ({ 0.2f, 0.2f, 0.2f }, { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { -0.2f, -1.0f, -0.3f });

        Light_Point light_point({ 0.2f, 0.2f, 0.2f }, { 1.0f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, 0.09f, 0.032f, sm_WhiteCube.position);


        //vector<Light_Point> light_point_pool;
        //light_point_pool.reserve(4);
        //for (size_t i = 0; i < 4; i++)
        //{
        //    light_point_pool.emplace_back(glm::vec3{ 0.2f, 0.2f, 0.2f }, glm::vec3{ 0.5f, 0.5f, 0.5f }, glm::vec3{ 1.0f, 1.0f, 1.0f }, 0.09f, 0.032f, pointLightPositions[i]);
        //}

        Light_Spotlight light_flashlight({ 0.0f, 0.0f, 0.0f }, { 0.7f, 1.0f, 1.2f }, { 1.0f, 1.0f, 1.0f }, 0.09f, 0.032f, camera1.cameraPos, camera1.cameraFront, 12.0f, 14.0f);

        //VertexArray vao;
        //VertexBuffer vbo(vertices.data(), sizeof(vertices));
        //vbo.Bind();
        //vbo.SetBufferData();

        //VertexBufferLayout layout{ 3.0f, 3.0f, 2.0f };

        //vao.AddBuffer(vbo, layout);

        //2 shaders (vertex and fragment) per file
        Shader shader_lighting("Shaders/Combined_Lighting.glsl");
        //Shader shader_basic_model("Shaders/Basic_Model.glsl");
        Shader shader_lightsource("Shaders/Lightsource.glsl");

        //shader_lighting.Unbind();
        //shader_basic_model.Unbind();
        //shader_lightsource.Unbind();
        //vbo.Unbind();
        //vao.Unbind();

        //VertexArray vao_lightsource;
        //VertexBuffer vbo(vertices.data(), sizeof(vertices));
        //vbo.Bind();
        //vbo.SetBufferData();

        VertexBufferLayout layout{ 3.0f, 3.0f, 2.0f };
        //vao_lightsource.AddBuffer(vbo, layout);


        //vbo.Unbind();
        //vao_lightsource.Unbind();

        //MAIN GAME LOOP
        while (!glfwWindowShouldClose(window->Get()))
        {
            Renderer::GLClear();
            Renderer::SetBackgroundColor(colors[Colors::DARK_TURQUOISE]);

            //CAMERA
            camera1.UpdateViewProjection();
            camera1.ProcessKeyboard();

            //LIGHTING
            //shader_lighting.Bind();

            //unsigned idx = 0;
            //for (auto& i : light_point_pool) 
            //{
            //    i.UseMany(shader, idx);
            //    idx++;
            //}

            //shader_lighting.SetUniformMatrix4fv("view", view);
            //shader_lighting.SetUniformMatrix4fv("projection", projection);

            //material1.Use(shader_lighting);
            //light_flashlight.Use(shader_basic_model);

            //MODELS
            //shader_basic_model.Bind();
            //shader_basic_model.SetUniformMatrix4fv("view", camera1.view);
            //shader_basic_model.SetUniformMatrix4fv("projection", camera1.projection);

            //DRAW CALLS AND TRANSFORMATIONS

            //model_backpack
            //glm::mat4 mat_model_backpack(1.0f);
            //mat_model_backpack = glm::translate(mat_model_backpack, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
            //mat_model_backpack = glm::scale(mat_model_backpack, glm::vec3(0.5f));	// for backpack

            //shader_basic_model.SetUniformMatrix4fv("model", mat_model_backpack);

            //model_backpack.Draw(shader_basic_model);

            //sm_WhiteCube
            shader_lightsource.Bind();
            shader_lightsource.SetUniformMatrix4fv("view", camera1.view);
            shader_lightsource.SetUniformMatrix4fv("projection", camera1.projection);

            float rotation_radius = 1.5f; 
            sm_WhiteCube.position.x = sin(glfwGetTime()) * rotation_radius;
            sm_WhiteCube.position.z = cos(glfwGetTime()) * rotation_radius;

            glm::mat4 mat_sm_WhieCube(1.0f);
            mat_sm_WhieCube = glm::translate(mat_sm_WhieCube, sm_WhiteCube.position);
            mat_sm_WhieCube = glm::scale(mat_sm_WhieCube, glm::vec3(0.2f));
            shader_lightsource.SetUniformMatrix4fv("model", mat_sm_WhieCube);

            sm_WhiteCube.Draw(shader_lightsource);

            //sm_Box
            shader_lighting.Bind();
            shader_lighting.SetUniformMatrix4fv("view", camera1.view);
            shader_lighting.SetUniformMatrix4fv("projection", camera1.projection);

            //TEST
            //shader_lighting.SetUniform1ui("number_of_texture_units", 1);

            glm::mat4 mat_sm_Box(1.0f);
            mat_sm_Box = glm::translate(mat_sm_Box, sm_Box.position);
            shader_lighting.SetUniformMatrix4fv("model", mat_sm_Box);

            sm_Box.Draw(shader_lighting);

            //model_backpack
            glm::mat4 mat_model_backpack(1.0f);
            mat_model_backpack = glm::translate(mat_model_backpack, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
            mat_model_backpack = glm::scale(mat_model_backpack, glm::vec3(0.5f));

            shader_lighting.SetUniformMatrix4fv("model", mat_model_backpack);

            light_directional.Use(shader_lighting, 0);
            light_flashlight.Use(shader_lighting, 1);
            light_point.Use(shader_lighting, 2);


            model_backpack.Draw(shader_lighting);
            
            //Swap front and back buffers
            glfwSwapBuffers(window->Get());

            glfwPollEvents();

#ifdef BENCHMARK_MODE_ON
            break;
#endif // BENCHMARK_MODE_ON
        }

    }
    glfwTerminate();

    return 0;
}

void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
{
    camera1.ProcessMouse(window, xpos, ypos);
}
void scroll_callback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset)
{
    camera1.ProcessMouseScroll(window, xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //for multiple keys pressed at the same time
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;

    if (keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (keys[GLFW_KEY_F])
        Renderer::ToggleWireFrameMode();

}
