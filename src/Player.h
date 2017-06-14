#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <glm/glm.hpp>
#include "obj.h"
#include "Shader.h"

class Player{
private:
    glm::vec3 Postion;
    static Shader shader;
    static GLuint VAO, verticesNum;
    
public:
    
    glm::vec3 getPos(){
        return Postion;
    }
    void Load();
    void Display();
};

#endif