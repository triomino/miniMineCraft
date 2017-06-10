#ifndef __Chunk_H__
#define __Chunk_H__

#include <stdlib.h>
#include <fstream>
#include <utility>
#include <map>
#include <set>
#include <glm/glm.hpp>
#include "FilePath.h"
#include "Random.h"
#include "Block.h"
#include "Player.h"

#define PII pair<int, int>

// declaration
class Chunk{
    friend class ChunkManager;
private:
    glm::vec3 Center;
    Block *block;
    glm:: vec2 Position;
    int blockNum;
    void randomProduce();
    Chunk *AdjChunk[4]; // 0-z, 1z, 2-x, 3x
    std::set<int> onDraw;
    
public:
    static const int xLength = 16;
    static const int zLength = 16;
    static const int yLength = 128;
    static const int xHalf = xLength >> 1;
    static const int yHalf = yLength >> 1;
    static const int zHalf = zLength >> 1;
    
    void Display();
    void Load(int x, int z);
    void WrittenBack();
    void CheckAdEmpty();
    
    Chunk(){
        block = new Block[blockNum = xLength * yLength * zLength];
        onDraw.clear();
    }
    ~Chunk(){
        WrittenBack();
        delete []block;
    }
    
};

class ChunkManager{
private:
    std::map<std::PII, int> ChunkPosMap;
    static const int num = 500;
    int r;
    bool loaded[num];
    Chunk chunk[num];
    
public:
    ChunkManager(int radius = 10){
        r = radius;
        for (int i = 0; i < num; i++){
            loaded[i] = false;
        }
        ChunkPosMap.clear();
    }
    
    //void Load();
    void CheckPos(glm::vec3 PlayerPos);
    void Display();
};


////////////////////////////////////////////////////////////////////////////////
// methods
//class Chunk

void Chunk::randomProduce(){
    Block *pb = block;
    for (int i = 0; i < xLength; i++){
        for (int j = 0, k, l; j < zLength; j++){
            k = RandomGetInt(10) + yHalf;
            for (l = 0; l < k; l++){
                (pb++)->bt = Soil;
            }
            (pb++)->bt = Grass;
            for (l = k + 1; l < yLength; l++){
                (pb++)->bt = Empty;
            }
        }
    }
}

void Chunk::CheckAdEmpty(){
    int dx = {0, 0, 0, 0, -1, 1};
    int dz = {0, 0, -1, 1, 0, 0};
    int dy = {-1, 1, 0, 0, 0, 0};
    const int is = zLength * yLength;
    const int js = yLength;
    Block *pb = block;
    for (int i = 0, j, k, d, temp; i < xLength; i++){
        for (j = 0; j < zLength; j++){
            for (k = 0; k < yLength; k++){
                if (pb->bt != Empty)
                    for (d = 0; d < 6; d++){
                        temp = (i + dx[d]) * is + (j + dz[d]) * js + k + dy[d];
                        if (block[temp].bt == Empty && set.find(temp) == set.end()){
                            onDraw.insert(temp);
                        }
                    }
                pb++;
            }
        }
    }
}

void Chunk:: Display(){
    for (int i = 0; i < blockNum; i++){
        block[i].Display();
    }
}

void Chunk::Load(int x, int z){
    
    Position = glm::vec2(x, z);
    
    char a[10], b[10];
    itoa(x, a, 10);
    itoa(z, b, 10);
    std::string FileName = ChunkFilePath + a + "_" + b;
    std::ifstream in;
    in.open(FileName.data());
    if (!in){
        randomProduce();
        WrittenBack();
    }
    else {
        for (int i = 0, temp; i < blockNum; i++){
            in >> temp;
            block[i].bt = BlockType(temp);
        }
        in.close();
    }
    CheckAdEmpty();
}

void Chunk::WrittenBack(){
    char a[10], b[10];
    itoa(Position.x, a, 10);
    itoa(Position.y, b, 10);
    std::string FileName = ChunkFilePath + a + "_" + b;
    std::ofstream out;
    out.open(FileName.data());
    std::string temp = "";
    for (int i = 0; i < blockNum; i++){
        temp += itoa((int(block[i].bt)), a, 10);
        if (i != blockNum - 1) temp += " ";
    }
    out << temp;
    out.close();
}

//End class Chunk
////////////////////////////////////////////////////////////////////////////////
//class Chunk Manager

void ChunkManager::CheckPos(glm::vec3 PlayerPos){
    static int lastx = 0;
    static int lasty = 0;
    static int firstCheck = 1;
    int x = PlayerPos.x / Chunk::xLength;
    int y = PlayerPos.z / Chunk::zLength;
    if (firstCheck){
        firstCheck = 0;
        lastx = x - 1;
    }
    
    // main check
    if (lastx != x || lasty != y){
        for (int i = 0; i < num; i++)
        if (loaded[i]){
            int dx = chunk[i].Position.x - x;
            int dy = chunk[i].Position.y - y;
            if (dx * dx + dy * dy > r * r){
                loaded[i] = false;
                ChunkPosMap.erase(std::make_pair(chunk[i].Position.x, chunk[i].Position.y));
                chunk[i].WrittenBack();
            }
        }
        for (int i = -r; i <= r; i++){
            for (int j = -r, k = 0; j <= r; j++){
                if (i * i + j * j <= r * r && ChunkPosMap.find(std::make_pair(x + i, y + j)) == ChunkPosMap.end()){
                    for (; k < num && loaded[k]; k++);
                    if (k < num){
                        loaded[k] = true;
                        chunk[k].Load(x + i, y + j);
                        ChunkPosMap[std::make_pair(x + i, y + j)] = k;
                    }
                }
            }
        }
        // set Adjacent Chunk
        const int dx[4] = {0, 0, -1, 1};
        const int dy[4] = {-1, 1, 0, 0};
        for (int i = 0, k, cx, cy, tox, toy; i < num; i++)
        if (loaded[i]){
            cx = chunk[i].Position.x;
            cy = chunk[i].Position.y;
            for (int d = 0; d < 4; d++){
                tox = cx + dx[d];
                toy = cy + dy[d];
                if (ChunkPosMap.find(std::make_pair(tox, toy)) != ChunkPosMap.end()){
                    k = ChunkPosMap[std::make_pair(tox, toy)];
                    chunk[i].AdjChunk[d] = chunk + k;
                }
                else {
                    chunk[i].AdjChunk[d] = NULL;
                }
            }
        }
    }
    
    lastx = x;
    lasty = y;
    // std::cout << num << std::endl;
    // for (int i = 0; i < num; i++)
    // if (loaded[i]){
        // std::cout << chunk[i].Position.x << " " << chunk[i].Position.y << std::endl;
    // }
    // std::cout << std::endl;
}

void ChunkManager::Display(){
    for (int i = 0; i < num; i++)
    if (loaded[i]){
        chunk[i].Display();
    }
}

// End class ChunkManager
#endif
