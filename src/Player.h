#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include "obj.h"
#include "Shader.h"
#include "Movement.h"

class Player{
private:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Right;
    GLfloat Yaw;
    float *vertices;
    int ClVerticesNum;
    glm::mat4 PlayerModelMatrix;
    
    
    static Shader shader;
    static GLuint VAO, verticesNum;
    static glm::vec3 offset;
    static GLfloat MovementSpeed;
    static GLfloat shininess;
    
public:
    static const float ScaleSize = 0.2258076009;
    static const float Height = 1.8f;
    
    
    ~Player();
    
    glm::vec3 getPos(){
        return Position;
    }
    void setFront(glm::vec3 cf){
        Front = cf;
    }
    
    void setRight(glm::vec3 cr){
        Right = cr;
    }
    void setYaw(GLfloat yaw){
        Yaw = yaw;
    }
    
    void Load();
    void Display();
    
    void ProcessKeyboard(const bool *keys, GLfloat deltaTime);
    bool onGround();
    
    void calPlayerModelMatrix();
    
    bool collision(glm::vec3 disp);
    bool collision(float x, float y, float z);
};

#endif