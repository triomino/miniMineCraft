#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <glm/glm.hpp>
class Player{
private:
    glm::vec3 Postion;
public:
    glm::vec3 getPos(){
        return Postion;
    }
    void Load(){}
    void Display(){}
};

#endif