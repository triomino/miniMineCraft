#ifndef __MODEL_H__
#define __MODEL_H__

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Image Processing
#include <SOIL.h>
// GLM Mathematics

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Random
#include "Random.h"

// Tools
#include "Shader.h"

// Game Objects
#include "Materia.h"
#include "Camera.h"
#include "Player.h"
#include "Block.h"
#include "Chunk.h"
#include "Light.h"

class Model{
private:
    static ChunkManager cm;
    static Player player;
    static Camera camera;
    // key state
    static bool keyPressed[1024];
    // window
    static GLFWwindow* window;
    
    // listeners
    static void (*key_callback)(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void (*mouse_callback)(GLFWwindow* window, double xpos, double ypos);
    static void (*scroll_callback)(GLFWwindow* window, double xoffset, double yoffset);
    static void (*mouse_button_callback)(GLFWwindow* window, int button, int action, int mods);
    
public:
    static GLfloat deltaTime;	// Time between current frame and last frame
    static GLfloat lastFrame;  	// Time of last frame
    
    // current Choosing Cube
    static bool hasChoosingCube;
    static std::PII ChoosingChunk;
    static int ChoosingCube;
    
    // current Choosing Cube
    static std::PII PuttingChunkPos;
    static int PuttingCube;
    static BlockType PuttingBT;
    
    // Check before drawing
    static void CheckPos();
    static void CheckChoosingCube();
    
    // check putting
    static void CheckPuttingCube();
    static int OP_MODE;
    
    // Sun Move
    static void SunMove();

    // init
    static void Init();
    
    // Entrance of drawing
    static void Display();

    // data access
    static Camera &getCamera(){ return camera; }
    static Player &getPlayer(){ return player; }
    
    // keyboard
    static void setKeyPressed(int key, bool value);
    const static bool *getKeyPressed();
    
    // mouse
    static void left_button_pressed();
    static void right_button_pressed();
    
    // window
    static GLFWwindow* getWindow(); 
    
    // Cube test
    static BlockType getBlockType(float x, float y, float z);
    static bool hasCube(float x, float y, float z);
    static bool hasCube(glm::vec3(pos));
    static BlockType getBlockType(glm::vec3(pos));
    
    // register
    static void KeyboardRegister(void (*k)(GLFWwindow* window, int key, int scancode, int action, int mode)){
        key_callback = k;
    }
    static void MouseRegister(void (*m)(GLFWwindow* window, double xpos, double ypos)){
        mouse_callback = m;
    }
    static void ScrollRegister(void (*s)(GLFWwindow* window, double xoffset, double yoffset)){
        scroll_callback = s;
    }
    static void MouseButtonRegister(void (*mb)(GLFWwindow* window, int button, int action, int mods)){
        mouse_button_callback = mb;
    }
};

#endif