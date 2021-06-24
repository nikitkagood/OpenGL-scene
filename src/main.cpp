#define STATIC_DRAW

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL/SOIL.h> //Simple OpenGL Image Library

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include "Optional/KHR_Debug.h"

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Light.h"
#include "Window.h"

const GLuint WIDTH = 1280, HEIGHT = 720;
bool keys[1024];

#include "Camera.h"

//not just camera
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

//TODO: LIGHT FADE DOESNT ACCOUT FOR LIGHTSOURCE STARTING LOCATION
//FAR LIGHT CREATES A LOT OF GLARE (SPECULAR LIGHTING) - check if this is how it should be

int main()
{
    using namespace std;

    glEnable(GL_DEBUG_OUTPUT);

    Renderer renderer;

    Window window(WIDTH, HEIGHT, 4);

    //Set the required callback functions
    window.SetKeyCallback(key_callback);
    window.SetCursorPosCallback(mouse_callback);
    window.SetScrollCallback(scroll_callback);

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window.Get(), &width, &height);
    glViewport(0, 0, width, height);

    cout << glGetString(GL_VERSION) << endl;

    //another scope to make glfwTerminate work correclty and not throwing an error when GL window is closed
    {
        array<float, 288> vertices = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };

        Texture tx_Container("Textures/container2.png");
        Texture tx_Container_map_specular("Textures/container2_specular.png");
        //Texture tx_What_for_cat("Textures/what_for_cat.png");

        Material material1({ 1.0f, 1.0f, 1.0f }, { 1.0f, 0.5f, 0.31f }, { 0.5f, 0.5f, 0.5f }, 32.0f);
        //Light light1({ 0.2f, 0.2f, 0.2f }, { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f });
        //Light_Directional light1({ 0.2f, 0.2f, 0.2f }, { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { -0.2f, -1.0f, -0.3f });
        //Light_Dot light1({ 0.2f, 0.2f, 0.2f }, { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, 0.09f, 0.032f);
        Light_Spotlight light1({ 0.2f, 0.2f, 0.2f }, { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, 0.09f, 0.032f, cameraPos, cameraFront, 15.5f, 17.5f);

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
        //Shader shader("Shaders/Model_Combined_Lighting.glsl"); //it's not complete
        Shader shader("Shaders/Model_Spotlight_Lighting.glsl"); //using shader that matches the light type and object above

        vbo.Unbind();
        shader.Unbind();
        vao.Unbind();

        glm::vec3 lightPos(0.7f, 0.3f, 3.0f);
        VertexArray vao_lightsource;

        vao_lightsource.AddBuffer(vbo, layout);

        Shader shader_lightsource("Shaders/Lightsource.glsl");

        shader_lightsource.Unbind();
        vao_lightsource.Unbind();


        glEnable(GL_DEPTH_TEST);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window.Get()))
        {
            //delta time calculation
            GLdouble currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            camera_movement();

            //clearing buffers for each frame to display correctly
            renderer.GLClear();

            shader.Bind(); //required before any transformations

            //TEXTURING
            tx_Container.Use(0, shader.GetID(), "material.diffuse");
            tx_Container_map_specular.Use(1, shader.GetID(), "material.specular");

            //LIGHTING
            //shader.SetUniform3f("objectColor", 1.0f, 0.5f, 0.31f);
            shader.SetUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
            shader.SetUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

            material1.Use(shader);
            light1.Use(shader);


            //CAMERA
            glm::mat4 view(1.0f);
            glm::mat4 projection(1.0f);

            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            projection = glm::perspective(glm::radians(FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

            shader.SetUniformMatrix4fv("view", view);
            shader.SetUniformMatrix4fv("projection", projection);

            //box
            glm::mat4 model_box1(1.0f);
            model_box1 = glm::translate(model_box1, { 0.0f, 0.0f, -0.0f });
            //GLfloat angle = 0;
            //model_box1 = glm::rotate(model_box1, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            shader.SetUniformMatrix4fv("model", model_box1);

            //DRAW CALLS START HERE
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
            //glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //glClearColor(0.2f, 0.3f, 0.3f, 1.0f); - dark green-blue color
            //renderer.DrawArrays(vao, shader, sizeof(vertices));
            renderer.DrawArrays(vao, shader, 36);

            glm::mat4 model_box2(1.0f);
            model_box2 = glm::translate(model_box2, { 1.5f, 0.0f, -5.0f });
            shader.SetUniformMatrix4fv("model", model_box2);

            renderer.DrawArrays(vao, shader, 36);

            glm::mat4 model_box3(1.0f);
            model_box3 = glm::translate(model_box3, { -2.2f, 0.0f, -5.0f });
            shader.SetUniformMatrix4fv("model", model_box3);

            renderer.DrawArrays(vao, shader, 36);

            vao.Unbind();


            //lamp cube
            shader_lightsource.Bind();

            shader_lightsource.SetUniformMatrix4fv("view", view);
            shader_lightsource.SetUniformMatrix4fv("projection", projection);

            glm::mat4 model_lamp_cube(1.0f);

            float rotation_radius = 1.5f; 
            lightPos.x = sin(glfwGetTime()) * rotation_radius;
            lightPos.z = cos(glfwGetTime()) * rotation_radius;

            model_lamp_cube = glm::translate(model_lamp_cube, lightPos);
            model_lamp_cube = glm::scale(model_lamp_cube, glm::vec3(0.2f));
            shader_lightsource.SetUniformMatrix4fv("model", model_lamp_cube);

            renderer.DrawArrays(vao_lightsource, shader_lightsource, 36);

            vao_lightsource.Unbind();


            //Swap front and back buffers
            glfwSwapBuffers(window.Get());

            //Sync with refresh rate
            //Renderer::VSync(true);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();

    return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //keys that don't belong to camera movement
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        //renderer.ToggleWireFrameMode();
        Renderer::ToggleWireFrameMode();

    //for multiple keys pressed at the same time
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}