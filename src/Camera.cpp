#include "Camera.h"

Camera::Camera(const Window& window, bool keys_array[]) : current_window(window), keys(keys_array), view(1.0f), projection(1.0f)
{
    window.SetCursorPosCallback(mouse_callback);
    window.SetScrollCallback(scroll_callback);
}

void Camera::CalculateDeltaTime()
{
    GLdouble currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Camera::UpdateViewProjection()
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    projection = glm::perspective(glm::radians(FOV), static_cast<float>(current_window.WIDTH) / static_cast<float>(current_window.HEIGHT), 0.1f, 100.0f);
}

void Camera::ProcessKeyboard() //it's NOT a callback, this function just checks keys[]; and keys[] itself is set by callback function in main
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

void Camera::ProcessMouse(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
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

void Camera::ProcessMouseScroll(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset)
{
    GLfloat sensitivity = 1.5f;
    GLfloat min_fov = 5.0f;
    GLfloat max_fov = 75.0f;

    if (FOV >= min_fov && FOV <= max_fov) FOV -= yoffset * sensitivity;
    if (FOV <= min_fov) FOV = min_fov;
    if (FOV >= max_fov) FOV = max_fov;
}