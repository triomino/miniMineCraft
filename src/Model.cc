#ifndef __MODEL_CC__
#define __MODEL_CC__

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Model.h"

ChunkManager Model::cm = ChunkManager(4);
Player Model::player;
Camera Model::camera;
bool Model::keyPressed[1024] = {false};
GLfloat Model::deltaTime = 0.0f;
GLfloat Model::lastFrame = 0.0f;
GLFWwindow *Model::window = NULL;
void (*Model::key_callback)(GLFWwindow* window, int key, int scancode, int action, int mode) = NULL;
void (*Model::mouse_callback)(GLFWwindow* window, double xpos, double ypos) = NULL;
void (*Model::scroll_callback)(GLFWwindow* window, double xoffset, double yoffset) = NULL;

void Model::Init(){
    Random::Init();
    GLuint WIDTH = Camera::WIDTH;
    GLuint HEIGHT = Camera::HEIGHT;
    
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow(WIDTH, HEIGHT, "Minecraft", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);
    cm.Load();
    player = Player();
    camera = Camera(glm::vec3(0.0f, 80.0f, 0.0f));
}

void Model::CheckPos(){
    cm.CheckPos(camera.Position);
}

void Model::Display(){
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    cm.Display();
}

Camera &Model::getCamera(){
    return camera;
}

void Model::setKeyPressed(int key, bool value){
    keyPressed[key] = value;
}

const bool *Model::getKeyPressed(){
    return (const bool*)keyPressed;
}

GLFWwindow* Model::getWindow(){
    return window;
}

#endif
