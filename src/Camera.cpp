/*
//CLASS PROTOTYPE

#include "Camera.h"

void Camera::camera_movement()
{
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

void Camera::mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
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

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    GLfloat sensitivity = 1.5f;
    GLfloat min_fov = 1.0f;
    GLfloat max_fov = 75.0f;

    if (FOV >= min_fov && FOV <= max_fov) FOV -= yoffset * sensitivity;
    if (FOV <= min_fov) FOV = min_fov;
    if (FOV >= max_fov) FOV = max_fov;
}

*/