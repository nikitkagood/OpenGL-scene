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


#include "Profiler.h"

//TODO LIST:
//Further development of color model
//Model.cpp:
// processNode - correct child-parent relations
// investigate  -> //if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
//VAO, VBO etc - check for excessive of binds/unbinds

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
        sm_WhiteCube.position = { 0.7f, 0.3f, 3.0f };

        //Light_Directional light_directional ({ 0.2f, 0.2f, 0.2f }, { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { -0.2f, -1.0f, -0.3f });

        //Light_Point light_point({ 0.2f, 0.2f, 0.2f }, { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, 0.09f, 0.032f);
        //glm::vec3 pointLightPositions[] = {
        //    glm::vec3(0.7f,  0.2f,  2.0f),
        //    glm::vec3(2.3f, -3.3f, -4.0f),
        //    glm::vec3(-4.0f, 2.0f, -12.0f),
        //    glm::vec3(0.0f,  0.0f, -3.0f)
        //};

        //vector<Light_Point> light_point_pool;
        //light_point_pool.reserve(4);
        //for (size_t i = 0; i < 4; i++)
        //{
        //    light_point_pool.emplace_back(glm::vec3{ 0.2f, 0.2f, 0.2f }, glm::vec3{ 0.5f, 0.5f, 0.5f }, glm::vec3{ 1.0f, 1.0f, 1.0f }, 0.09f, 0.032f, pointLightPositions[i]);
        //}

        //glm::vec3 lightPos(0.7f, 0.3f, 3.0f);
        //Material material1({ 1.0f, 1.0f, 1.0f }, { 1.0f, 0.5f, 0.31f }, { 0.5f, 0.5f, 0.5f }, 32.0f);
        //Material material1({ 1.0f, 0.5f, 0.31f }, { 0.5f, 0.5f, 0.5f }, 32.0f);
        //Light_Spotlight light_flashlight({ 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, 0.09f, 0.032f, lightPos, camera1.cameraFront, 12.0f, 14.0f); //flashlight

        //VertexArray vao;
        //VertexBuffer vbo(vertices.data(), sizeof(vertices));
        //vbo.Bind();
        //vbo.SetBufferData();

        //VertexBufferLayout layout{ 3.0f, 3.0f, 2.0f };

        //vao.AddBuffer(vbo, layout);

        //2 shaders (vertex and fragment) per file
        //Shader shader_lighting("Shaders/Combined_Lighting.glsl");
        Shader shader_basic_model("Shaders/Basic_Model.glsl");

        //shader_lighting.Unbind();
        shader_basic_model.Unbind();
        //vbo.Unbind();
        //vao.Unbind();

        //VertexArray vao_lightsource;
        //VertexBuffer vbo(vertices.data(), sizeof(vertices));
        //vbo.Bind();
        //vbo.SetBufferData();

        VertexBufferLayout layout{ 3.0f, 3.0f, 2.0f };
        //vao_lightsource.AddBuffer(vbo, layout);

        Shader shader_lightsource("Shaders/Lightsource.glsl");

        shader_lightsource.Unbind();
        //vbo.Unbind();
        //vao_lightsource.Unbind();

        
        //MAIN GAME LOOP
        while (!glfwWindowShouldClose(window->Get()))
        {
            Renderer::GLClear();

            camera1.UpdateViewProjection();
            camera1.CalculateDeltaTime();
            camera1.ProcessKeyboard();

            //CAMERA
            //glm::mat4 view(1.0f);
            //glm::mat4 projection(1.0f);
            //camera1.view = glm::lookAt(camera1.cameraPos, camera1.cameraPos + camera1.cameraFront, camera1.cameraUp);
            //camera1.projection = glm::perspective(glm::radians(camera1.FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

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
            shader_basic_model.Bind();
            shader_basic_model.SetUniformMatrix4fv("view", camera1.view);
            shader_basic_model.SetUniformMatrix4fv("projection", camera1.projection);


            //DRAW CALLS AND TRANSFORMATIONS
            Renderer::SetBackgroundColor(colors[Colors::TURQUOISE]);

            //backpack
            glm::mat4 matrix_model_backpack(1.0f);
            matrix_model_backpack = glm::translate(matrix_model_backpack, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
            matrix_model_backpack = glm::scale(matrix_model_backpack, glm::vec3(0.5f));	// for backpack

            shader_basic_model.SetUniformMatrix4fv("model", matrix_model_backpack);
            model_backpack.Draw(shader_basic_model);
            //model_backpack.Draw(shader_lighting);


            //lamp cube
            shader_lightsource.Bind();
            shader_lightsource.SetUniformMatrix4fv("view", camera1.view);
            shader_lightsource.SetUniformMatrix4fv("projection", camera1.projection);

            float rotation_radius = 1.5f; 
            sm_WhiteCube.position.x = sin(glfwGetTime()) * rotation_radius;
            sm_WhiteCube.position.z = cos(glfwGetTime()) * rotation_radius;

            glm::mat4 matrix_lamp_cube(1.0f);
            matrix_lamp_cube = glm::translate(matrix_lamp_cube, sm_WhiteCube.position);
            matrix_lamp_cube = glm::scale(matrix_lamp_cube, glm::vec3(0.2f));
            shader_lightsource.SetUniformMatrix4fv("model", matrix_lamp_cube);
            sm_WhiteCube.Draw(shader_lightsource);
            
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
