#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <glm/glm.hpp>

enum BlockType{
    Empty,
    Grass,
    Soil,
    BlockTypeNum
};


class Block{
friend class Chunk;
private:
    BlockType bt;
public:
    Block(){}
    ~Block(){}
    void Display(){}
};

#endif