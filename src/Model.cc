#ifndef __MODEL_CC__
#define __MODEL_CC__

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <cmath>

#include "Model.h"

// Game Objects
ChunkManager Model::cm;
Player Model::player;
Camera Model::camera;

// keyboard 
bool Model::keyPressed[1024] = {false};
// time system
GLfloat Model::deltaTime = 0.0f;
GLfloat Model::lastFrame = 0.0f;

// window
GLFWwindow *Model::window = NULL;

// ChoosingCube
bool Model::hasChoosingCube;
std::PII Model::ChoosingChunk;
int Model::ChoosingCube;

std::PII Model::PuttingChunkPos;
int Model::PuttingCube;
BlockType Model::PuttingBT = Dirt;

int Model::OP_MODE = 0;

// listeners
void (*Model::key_callback)(GLFWwindow* window, int key, int scancode, int action, int mode) = NULL;
void (*Model::mouse_callback)(GLFWwindow* window, double xpos, double ypos) = NULL;
void (*Model::scroll_callback)(GLFWwindow* window, double xoffset, double yoffset) = NULL;
void (*Model::mouse_button_callback)(GLFWwindow* window, int button, int action, int mods) = NULL;


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
    glfwSetMouseButtonCallback(window, mouse_button_callback); 

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
    Materia::Load();
    cm.Load();
    Light::Load();
    
    player.Load();
    camera = Camera(glm::vec3(0.0f, 65.8f, 0.0f));
    camera.syncPlayer(player);
}

void Model::CheckPos(){
    cm.CheckPos();
}

void Model::Display(){
    float x = 0.0f;
    float y = 127.0f / 255.0f;
    float z = 1.0f;
    
    float coe = 1.0f - fabs(Light::SunAngle - 90.0f) / 90.0f;
    if (coe < 0.0f) coe = 0.0f;
    
    glClearColor(coe * x, coe * y, coe * z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    camera.syncPlayer(player);
    cm.Display();
    Light::Display();
    player.Display();
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

void Model::left_button_pressed(){
    if (OP_MODE == 0) if (hasChoosingCube) cm.RemoveCube(ChoosingChunk, ChoosingCube);
    if (OP_MODE == 1){
        cm.AddCube(PuttingChunkPos, PuttingCube, PuttingBT);
    }
}

void Model::right_button_pressed(){
    
}

void Model::CheckChoosingCube(){
    hasChoosingCube = false;
    glm::vec3 step = glm::normalize(camera.Front) / 10.0f;
    glm::vec3 testPoint = camera.Position + step;
    int xl = Chunk::xLength;
    int yl = Chunk::yLength;
    int zl = Chunk::zLength;
    unsigned int bn;
    for (int i = 0, x, z, cx, cz, dx, dz; i < 50; i++, testPoint += step){
        x = round(testPoint.x);
        z = round(testPoint.z);
        cx = (x + (x < 0)) / xl - (x < 0);
        cz = (z + (z < 0)) / zl - (z < 0);
        dx = x - cx * xl;
        dz = z - cz * zl;
        bn = dx * zl * yl + dz * yl + round(testPoint.y);
        
        if (cm.getBlockType(std::make_pair(cx, cz), bn) != Empty){
            hasChoosingCube = true;
            ChoosingChunk = std::make_pair(cx, cz);
            ChoosingCube = bn;
            break;
        }
    }
}

void Model::CheckPuttingCube(){
    glm::vec3 step = glm::normalize(camera.Front) / 10.0f;
    glm::vec3 testPoint = camera.Position + step;
    int xl = Chunk::xLength;
    int yl = Chunk::yLength;
    int zl = Chunk::zLength;
    unsigned int bn;
    int x, z, cx, cz, dx, dz;
    for (int i = 0; i < 50; i++, testPoint += step){
        x = round(testPoint.x);
        z = round(testPoint.z);
        cx = (x + (x < 0)) / xl - (x < 0);
        cz = (z + (z < 0)) / zl - (z < 0);
        dx = x - cx * xl;
        dz = z - cz * zl;
        bn = dx * zl * yl + dz * yl + round(testPoint.y);
        PuttingChunkPos = std::make_pair(cx, cz);
        PuttingCube = bn;
        if (cm.getBlockType(std::make_pair(cx, cz), bn) != Empty){
            break;
        }
    }
}

void Model::SunMove(){
    Light::SunMove();
}

BlockType Model::getBlockType(float x, float y, float z){
    return cm.getBlockType(glm::vec3(x, y, z));
}

bool Model::hasCube(glm::vec3 pos){
    BlockType bt = cm.getBlockType(pos);
    return bt != Empty && bt != Water && bt != Cloud;
}

bool Model::hasCube(float x, float y, float z){
    BlockType bt = getBlockType(x, y, z);
    return bt != Empty && bt != Water && bt != Cloud;
}


#endif
