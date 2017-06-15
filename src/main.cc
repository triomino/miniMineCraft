#include <iostream>
#include <fstream>
#include <cmath>
#include <set>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// OpenGL
#include "Shader.h"
#include "Camera.h"

// Random
#include "Random.h"

// Game
#include "Model.h"
#include "Control.h"

//#define boooom

// Window dimensions
const GLuint WIDTH = 1024, HEIGHT = 768;

//#define boooom
// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
// The MAIN function, from here we start the application and run the game loop
int main()
{
    #ifdef boooom
        std::ofstream out;
        out.open("er.txt");
        out << "boooom" << std::endl;
    #endif 
    Model::KeyboardRegister(key_callback);
    Model::MouseRegister(mouse_callback);
    Model::ScrollRegister(scroll_callback);
    Model::MouseButtonRegister(mouse_button_callback);
    Model::Init();
    GLFWwindow *window = Model::getWindow();

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        Model::deltaTime = currentFrame - Model::lastFrame;
        Model::lastFrame = currentFrame;
        
        Model::CheckPos();
        Model::CheckChoosingCube();
        Model::SunMove();
        #ifdef boooom
            glm::vec3 p = Model::ChoosingCube;
            if (Model::hasChoosingCube) out << p.x << " " << p.y << " " << p.z << std::endl;
        #endif

     
        glfwPollEvents();
        do_movement();

        Model::Display();


        glfwSwapBuffers(window);
    }
    
    /*std::ofstream out;
    out.open("er.txt");
    int k = 0;
    for (int i = 0; i < cm.num; i++){
        if (cm.chunk[i].Position.x == 0 && cm.chunk[i].Position.y == -1){
            k = i;
            break;
        }
    }
    for (int i = 0; i < 4; i++)
    if (cm.chunk[k].AdjChunk[i]){
        out << i << " " << cm.chunk[k].AdjChunk[i]->Position.x << " " << cm.chunk[k].AdjChunk[i]->Position.y << std::endl;
    }
    */
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    #ifdef boooom
        out.close();
    #endif 
    return 0;
}

