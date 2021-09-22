#pragma once

//Camera.h requires WIDTH, HEIGHT constants and bool keys[]!
//they are in main.cpp currently

 //key state array; used to track if a key is pressed so multiple keys can be tracked at the same time


#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//forward declaration
//bool keys[];

class Camera
{
private:
    bool* keys; //contains states for all keys; must be declared outside

    const GLuint WIDTH;
    const GLuint HEIGHT;

    GLdouble deltaTime = 0.0; //time elapsed between the last and the current frame
    GLdouble lastFrame = 0.0;

    //setting up camera; it's here for user input in key_callback
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    GLdouble yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
    GLdouble pitch = 0.0f;

    //last mouse coordinates, initialized with sceen center coordinates
    GLdouble lastX = WIDTH / 2.0;
    GLdouble lastY = HEIGHT / 2.0;
    GLboolean firstMouse = true;

    GLfloat FOV = 45.0f; //in degrees; translated into radians
public:
    Camera(bool keys_array[], GLuint resolution_width, GLuint resolution_height) : keys(keys_array), WIDTH(resolution_width), HEIGHT(resolution_height)
    {}

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void camera_movement() //sets things according to boolean array keys[]; the callback function itself is key_callback in main.cpp
    {
        //camera controls
        GLfloat cameraSpeed = 5.0f * deltaTime;
        if (keys[GLFW_KEY_W])
            cameraPos += cameraFront * cameraSpeed;
        if (keys[GLFW_KEY_S])
            cameraPos -= cameraFront * cameraSpeed;
        if (keys[GLFW_KEY_A])
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (keys[GLFW_KEY_D])
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (keys[GLFW_KEY_SPACE])
            cameraPos += cameraUp * cameraSpeed;
        if (keys[GLFW_KEY_LEFT_CONTROL])
            cameraPos -= cameraUp * cameraSpeed;
    }

    void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        GLdouble xoffset = xpos - lastX;
        GLdouble yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
        lastX = xpos;
        lastY = ypos;

        GLdouble sensitivity = 0.1;	// Change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front(1.0);
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        GLfloat sensitivity = 1.5f;
        GLfloat min_fov = 1.0f;
        GLfloat max_fov = 75.0f;

        if (FOV >= min_fov && FOV <= max_fov) FOV -= yoffset * sensitivity;
        if (FOV <= min_fov) FOV = min_fov;
        if (FOV >= max_fov) FOV = max_fov;
    }
};



GLdouble deltaTime = 0.0; //time elapsed between the last and the current frame
GLdouble lastFrame = 0.0;

//setting up camera; it's here for user input in key_callback
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void camera_movement()
{
    //camera controls
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_W])
        cameraPos += cameraFront * cameraSpeed;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraFront * cameraSpeed;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_SPACE])
        cameraPos += cameraUp * cameraSpeed;
    if (keys[GLFW_KEY_LEFT_CONTROL])
        cameraPos -= cameraUp * cameraSpeed;
}



GLdouble yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLdouble pitch = 0.0f;

//last mouse coordinates, initialized with sceen center coordinates
GLdouble lastX = WIDTH / 2.0;
GLdouble lastY = HEIGHT / 2.0;
bool firstMouse = true;



void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLdouble xoffset = xpos - lastX;
    GLdouble yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
    lastX = xpos;
    lastY = ypos;

    GLdouble sensitivity = 0.1;	// Change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front(1.0);
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

GLfloat FOV = 45.0f; //in degrees; translated into radians

//Perspective must use radians: glm::perspective(glm::radians(FOV), ...). Otherwise it breaks
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    GLfloat sensitivity = 1.5f;
    GLfloat min_fov = 1.0f;
    GLfloat max_fov = 75.0f;

    if (FOV >= min_fov && FOV <= max_fov) FOV -= yoffset * sensitivity;
    if (FOV <= min_fov) FOV = min_fov;
    if (FOV >= max_fov) FOV = max_fov;
}