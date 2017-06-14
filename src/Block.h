#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <glm/glm.hpp>


enum BlockType{
    Empty,
    Grass,
    Dirt,
    Cloud,
    BlockTypeNum
};


class Block{
friend class Chunk;
friend class ChunkManager;
private:
    BlockType bt;
public:
    Block(){
        bt = Empty;
    }
    ~Block(){
        
    }
};

#endif