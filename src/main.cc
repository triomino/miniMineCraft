#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include "Random.h"
#include "Player.h"
#include "Chunk.h"

int main(){
    RandomInit();
    ChunkManager cm(3);
    Player p;
    for (int i = 0; i < 10; i++){
        glm::vec3 Pos(i * 5, i * 5, i * 5);
        cm.CheckPos(Pos);
    }
}