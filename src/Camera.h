#pragma once

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
void scroll_callback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset);

class Camera
{
private:
    bool* keys; //contains states for all keys; must be declared outside; used to track if a key is pressed so multiple keys can be tracked at the same time

    Window& current_window;

    //last mouse coordinates, initialized with sceen center coordinates
    GLdouble lastX = current_window.WIDTH / 2.0;
    GLdouble lastY = current_window.HEIGHT / 2.0;
    GLboolean firstMouse = true;

    GLdouble yaw = -90.0;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
    GLdouble pitch = 0.0f;
public:
    GLdouble deltaTime = 0.0; //time elapsed between the last and the current frame
    GLdouble lastFrame = 0.0;

    //setting up camera; it's here for user input in key_callback
    glm::vec3 cameraPos{ 0.0f, 0.0f, 3.0f };
    glm::vec3 cameraFront{ 0.0f, 0.0f, -1.0f };
    glm::vec3 cameraUp{ 0.0f, 1.0f, 0.0f };

    GLfloat FOV = 45.0f; //in degrees; translated into radians

    GLdouble camera_speed_multiplier = 5.0;
    GLdouble camera_sensitivity_multiplier = 0.1;

    Camera(Window& window, bool keys_array[]) : current_window(window), keys(keys_array)
    {
        window.SetCursorPosCallback(mouse_callback);
        window.SetScrollCallback(scroll_callback);
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void CalculateDeltaTime()
    {
        GLdouble currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    void ProcessKeyboard() //it's NOT a callback, this function just checks keys[]; and keys[] itself is set by callback function in main
    {
        GLfloat cameraSpeed = camera_speed_multiplier * deltaTime;
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

    void ProcessMouse(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
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

        xoffset *= camera_sensitivity_multiplier;
        yoffset *= camera_sensitivity_multiplier;

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


    void ProcessMouseScroll(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset)
    {
        GLfloat sensitivity = 1.5f;
        GLfloat min_fov = 5.0f;
        GLfloat max_fov = 75.0f;

        if (FOV >= min_fov && FOV <= max_fov) FOV -= yoffset * sensitivity;
        if (FOV <= min_fov) FOV = min_fov;
        if (FOV >= max_fov) FOV = max_fov;
    }
};