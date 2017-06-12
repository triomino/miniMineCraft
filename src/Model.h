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

// Game Objects
#include "Chunk.h"
#include "Player.h"
#include "Camera.h"
#include "Shader.h"

class Model{
private:
    static ChunkManager cm;
    static Player player;
    static Camera camera;
    // key state
    static bool keyPressed[1024];
    // window
    static GLFWwindow* window;
    // callback functions
    static void (*key_callback)(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void (*mouse_callback)(GLFWwindow* window, double xpos, double ypos);
    static void (*scroll_callback)(GLFWwindow* window, double xoffset, double yoffset);
    
public:
    static void Init();
    static void CheckPos();
    static void Display();
    static Camera &getCamera();
    static GLfloat deltaTime;	// Time between current frame and last frame
    static GLfloat lastFrame;  	// Time of last frame
    
    // keyboard
    static void setKeyPressed(int key, bool value);
    const static bool *getKeyPressed();
    
    // window
    static GLFWwindow* getWindow();
    
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
};

#endif