#ifndef __CHUNK_H__
#define __CHUNK_H__

#include <stdio.h>
#include <cstdlib>
#include <queue>
#include <utility>
#include <algorithm>
#include <map>
#include <set>
#include <glm/glm.hpp>
#include "FilePath.h"
#include "Random.h"
#include "Model.h"

#define PII pair<int, int>
#define Square(a) ((a) * (a))
#define SquareSum(a,b) (Square(a) + Square(b))
#define Modu(p) SquareSum(p.first,p.second)

class Chunk{
    friend class ChunkManager;
private:
    glm::vec3 Center;
    Block *block;
    glm:: vec2 Position;
    
    void randomProduce();
    
    Chunk *AdjChunk[4]; // 0-z, 1z, 2-x, 3x
    
    std::set<int> onDraw;
    
    static const int FileSize = 131072;
    static char temp[FileSize];
    
public:
    static const int xLength = 16;
    static const int zLength = 16;
    static const int yLength = 128;
    static const int xHalf = xLength >> 1;
    static const int yHalf = yLength >> 1;
    static const int zHalf = zLength >> 1;
    static const int blockNum = xLength * yLength * zLength;
    void Display();
    void Load(int x, int z);
    void WrittenBack();
    void CheckAdEmpty();
    
    Chunk(){
        block = new Block[blockNum];
        onDraw.clear();
        for (int i = 0; i < 4; i++){
            AdjChunk[i] = NULL;
        }
    }
    ~Chunk(){
        delete [] block;
    }
    
};

enum ChunkEvent{
    ceWrittenBack,
    ceLoad,
    ceCheckAdEmpty,
    ceEventNum
};


class ChunkManager{
    friend class Chunk;
    friend class Light;
private:
    std::map<std::PII, int> ChunkPosMap;
    static const int num = 500;
    int r;
    bool loaded[num];
    Chunk chunk[num];
    // Event System
    int EventTimeHead, EventTimeTail;
    std::queue<std::queue<int> > EventQue; // Event: <Time, ChunkID, Type, ...>
    bool EventMap[num][ceEventNum];
    
    // Shader and vertices data of a cube
    static Shader shader;
    static GLfloat vertices[];
    static GLuint VBO, containerVAO;
    // texture map
    GLuint diffuseMap, specularMap, emissionMap;
    
public:
    ChunkManager(int radius = 5);
    ~ChunkManager();
    
    void Load();
    void CheckPos();
    void Display();
    // Data access
    BlockType getBlockType(std::PII cn, unsigned int bn);
    void RemoveCube(std::PII cPos, unsigned int bn);
    // Event System
    void RunEvent();
    void PushEvent(int time, int chunkId, ChunkEvent type, int x = 0, int y = 0);
};

#endif