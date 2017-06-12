#ifndef __CONTROL_CC__
#define __CONTROL_CC__

#include "Control.h"
#include "Model.h"

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Model::setKeyPressed(key, true);
        else if (action == GLFW_RELEASE)
            Model::setKeyPressed(key, false);
    }
}

void do_movement()
{
    Camera &camera = Model::getCamera();
    // Camera controls
    const bool *keys = Model::getKeyPressed();
    
    
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, Model::deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, Model::deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, Model::deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, Model::deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Camera &camera = Model::getCamera();
    static GLfloat lastX  =  camera.WIDTH  / 2.0;
    static GLfloat lastY  =  camera.HEIGHT / 2.0;
    static bool firstMouse = true;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera & camera = Model::getCamera();
    camera.ProcessMouseScroll(yoffset);
}

#endif