#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"

void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
void scroll_callback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset);

class Camera
{
public:
    glm::mat4 view;
    glm::mat4 projection;

    GLdouble deltaTime = 0.0; //time elapsed between the last and the current frame
    GLdouble lastFrame = 0.0;

    //setting up camera; it's here for user input in key_callback
    glm::vec3 cameraPos{ 0.0f, 0.0f, 3.0f };
    glm::vec3 cameraFront{ 0.0f, 0.0f, -1.0f };
    glm::vec3 cameraUp{ 0.0f, 1.0f, 0.0f };

    GLfloat FOV = 45.0f; //in degrees; translated into radians

    GLdouble camera_speed_multiplier = 5.0;
    GLdouble camera_sensitivity_multiplier = 0.1;

    Camera(const Window& window, bool keys_array[]);

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void UpdateViewProjection();

    void CalculateDeltaTime();

    void ProcessKeyboard(); //it's NOT a callback, this function just checks keys[]; and keys[] itself is set by callback function in main

    void ProcessMouse(GLFWwindow* window, GLdouble xpos, GLdouble ypos);

    void ProcessMouseScroll(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset);
private:
    bool* keys; //contains states for all keys; must be declared outside; used to track if a key is pressed so multiple keys can be tracked at the same time

    const Window& current_window;

    //last mouse coordinates, initialized with sceen center coordinates
    GLdouble lastX = current_window.WIDTH / 2.0;
    GLdouble lastY = current_window.HEIGHT / 2.0;
    GLboolean firstMouse = true;

    GLdouble yaw = -90.0;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
    GLdouble pitch = 0.0f;

};