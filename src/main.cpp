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

const GLuint WIDTH = 1280, HEIGHT = 720;
bool keys[1024]; //contains statuses if pressed for all the keys; used to implement multiple keys input 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

Renderer renderer;

Window window(WIDTH, HEIGHT); 
Camera camera1(window, keys);

int main()
{
#ifdef BENCHMARK_MODE_ON
    LOG_DURATION("BENCHMARK: main, 1 game cycle iteration");
#endif // BENCHMARK_MODE_ON

    using namespace std;

    glEnable(GL_DEBUG_OUTPUT);

    //Set the required callback functions
    window.SetKeyCallback(key_callback);

    //Define the viewport dimensions
    int vieport_width, vieport_height;
    glfwGetFramebufferSize(window.Get(), &vieport_width, &vieport_height);
    glViewport(0, 0, vieport_width, vieport_height);

    cout << glGetString(GL_VERSION) << endl;

    //another scope to make glfwTerminate work correclty and not throwing an error when GL window is closed
    {
        //for cube model without IBO, each 6 lines means one side of 2 triangles
        array<float, 288> vertices = {
            // positions          // normals                // texture coords
            -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
                                                           
            -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
                                                           
            -0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
                                                           
             0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
                                                           
            -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
                                                           
            -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f
        };

        Model model1("Models/backpack/backpack.obj"); 
        //Model model1("Models/table/table.obj"); //must be scaled down at least to 0.05f, 0.05f, 0.05f

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

        glm::vec3 lightPos(0.7f, 0.3f, 3.0f);
        //Material material1({ 1.0f, 1.0f, 1.0f }, { 1.0f, 0.5f, 0.31f }, { 0.5f, 0.5f, 0.5f }, 32.0f);
        Material material1({ 1.0f, 0.5f, 0.31f }, { 0.5f, 0.5f, 0.5f }, 32.0f);
        Light_Spotlight light_spotlight({ 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, 0.09f, 0.032f, lightPos, camera1.cameraFront, 12.0f, 14.0f);

        //for IBO/EBO
        //2 triangles -> square (that looks like rectangle)
        //array<unsigned, 6> indices =
        //{
        //    0, 1, 2,
        //    2, 3, 0
        //};

        VertexArray vao;
        VertexBuffer vbo(vertices.data(), sizeof(vertices));
        vbo.Bind();
        vbo.SetBufferData();

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);

        vao.AddBuffer(vbo, layout);

        //IndexBuffer ibo(indices.data(), indices.size());

        //2 shaders (vertex and fragment) per file
        Shader shader_lighting("Shaders/Combined_Lighting.glsl");
        Shader shader_basic_model("Shaders/Basic_Model.glsl");

        shader_lighting.Unbind();
        shader_basic_model.Unbind();
        vbo.Unbind();
        vao.Unbind();


        VertexArray vao_lightsource;
        vao_lightsource.AddBuffer(vbo, layout);

        Shader shader_lightsource("Shaders/Lightsource.glsl");

        shader_lightsource.Unbind();
        vao_lightsource.Unbind();

        glEnable(GL_DEPTH_TEST);

while (!glfwWindowShouldClose(window.Get()))
{
    //delta time calculation
    GLdouble currentFrame = glfwGetTime();
    camera1.deltaTime = currentFrame - camera1.lastFrame;
    camera1.lastFrame = currentFrame;

    camera1.ProcessKeyboard();

    //clearing buffers for each frame to display things correctly
    renderer.GLClear();

    //CAMERA
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);
    view = glm::lookAt(camera1.cameraPos, camera1.cameraPos + camera1.cameraFront, camera1.cameraUp);
    projection = glm::perspective(glm::radians(camera1.FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

     //LIGHTING
    shader_lighting.Bind();

    //unsigned idx = 0;
    //for (auto& i : light_point_pool) 
    //{
    //    i.UseMany(shader, idx);
    //    idx++;
    //}

    shader_lighting.SetUniformMatrix4fv("view", view);
    shader_lighting.SetUniformMatrix4fv("projection", projection);

    material1.Use(shader_lighting);
    light_spotlight.Use(shader_lighting);

    //MODELS
    shader_basic_model.Bind();

    shader_basic_model.SetUniformMatrix4fv("view", view);
    shader_basic_model.SetUniformMatrix4fv("projection", projection);

    //DRAW CALLS START HERE
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //0.2f, 0.3f, 0.3f, 1.0f - dark green-blue color

    glm::mat4 matrix_model1(1.0f);
    matrix_model1 = glm::translate(matrix_model1, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    matrix_model1 = glm::scale(matrix_model1, glm::vec3(0.5f));	// for backpack
    //matrix_model1 = glm::scale(matrix_model1, glm::vec3(0.05f, 0.05f, 0.05f));	// for table
    //matrix_model1 = glm::scale(matrix_model1, glm::vec3(0.005f, 0.005f, 0.005f));	// for street light
    shader_basic_model.SetUniformMatrix4fv("model", matrix_model1);
    model1.Draw(shader_basic_model);
    //model1.Draw(shader_lighting);

    //lamp cube
    shader_lightsource.Bind();
    shader_lightsource.SetUniformMatrix4fv("view", view);
    shader_lightsource.SetUniformMatrix4fv("projection", projection);

    float rotation_radius = 1.5f; 
    lightPos.x = sin(glfwGetTime()) * rotation_radius;
    lightPos.z = cos(glfwGetTime()) * rotation_radius;

    glm::mat4 model_lamp_cube(1.0f);
    model_lamp_cube = glm::translate(model_lamp_cube, lightPos);
    model_lamp_cube = glm::scale(model_lamp_cube, glm::vec3(0.2f));
    shader_lightsource.SetUniformMatrix4fv("model", model_lamp_cube);

    renderer.DrawArrays(vao_lightsource, shader_lightsource, 36);
    //vao_lightsource.Unbind();
    
    //for (size_t i = 0; i < 4; i++)
    //{
    //    glm::mat4 model_lamp_cube(1.0f);

    //    model_lamp_cube = glm::translate(model_lamp_cube, pointLightPositions[i]);
    //    model_lamp_cube = glm::scale(model_lamp_cube, glm::vec3(0.2f));
    //    shader_lightsource.SetUniformMatrix4fv("model", model_lamp_cube);

    //    renderer.DrawArrays(vao, shader_lighting, 36);
    //}

    //vao.Unbind();


    //Swap front and back buffers
    glfwSwapBuffers(window.Get());

    //Sync with refresh rate
    //Renderer::VSync(true);

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
