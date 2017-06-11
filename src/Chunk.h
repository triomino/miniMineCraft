#ifndef __Chunk_H__
#define __Chunk_H__

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
#include "Block.h"
#include "Player.h"

#define PII pair<int, int>

// declaration
class Chunk{
    friend class ChunkManager;
public:
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
        delete []block;
    }
    
};

char Chunk::temp[Chunk::FileSize] = "";

//class Chunk methods

void Chunk::randomProduce(){
    Block *pb = block;
    //int k = yHalf + RandomGetInt(10);
    int k = yHalf;
    for (int i = 0; i < xLength; i++){
        for (int j = 0, l; j < zLength; j++){
            for (l = 0; l < k; l++){
                (pb++)->bt = Soil;
            }
            (pb++)->bt = Grass;
            for (l = k + 1; l < yLength; l++){
                (pb++)->bt = Empty;
            }
            //k = yHalf + RandomGetInt(10);
            int temp = RandomGetInt(-10, 11);
            if (temp < -8) k--;
            if (temp > 8) k++;
            if (k < yHalf - 3) k = yHalf - 3;
            if (k > yHalf + 3) k = yHalf + 3;
        }
    }
}

#ifdef boooom
int tttt = 0;
#endif


void Chunk::CheckAdEmpty(){
    const int dx[] = {0, 0, 0, 0, -1, 1};
    const int dz[] = {0, 0, -1, 1, 0, 0};
    const int dy[] = {-1, 1, 0, 0, 0, 0};
    const int is = zLength * yLength;
    const int js = yLength;
    int cnt = 0;
    Block *pb = block;
    Block *pad;
    onDraw.clear();
    for (int i = 0, j, k, d; i < xLength; i++){
        for (j = 0; j < zLength; j++){
            for (k = 0; k < yLength; k++, cnt++, pb++){
                if (pb->bt != Empty && onDraw.find(cnt) == onDraw.end()){
                    pad = NULL;
                    for (d = 0; d < 6; d++)
                    if (k + dy[d] >= 0 && k + dy[d] < yLength){
                        if (i + dx[d] < 0){
                            if (AdjChunk[2])
                            pad = AdjChunk[2]->block + cnt + is * (xLength - 1);
                        }
                        else if (i + dx[d] >= xLength){
                            if (AdjChunk[3])
                            pad = AdjChunk[3]->block + cnt - is * (xLength - 1);
                        }
                        else if (j + dz[d] < 0){
                            if (AdjChunk[0])
                            pad = AdjChunk[0]->block + cnt + js * (zLength - 1);
                        }
                        else if (j + dz[d] >= zLength){
                            if (AdjChunk[1])
                            pad = AdjChunk[1]->block + cnt - js * (zLength - 1);
                        }
                        else {
                            pad = block + cnt + dx[d] * is + dz[d] * js + dy[d];
                        }
                        
                        if (pad && pad->bt == Empty){
                            break;
                        }
                    }
                    if (pad && pad->bt == Empty){
                        onDraw.insert(cnt);
                    }
                }
            }
        }
    }
    #ifdef boooom
    tttt++;
    std::ofstream out;
    out.open("er.txt", std::ios::app);
    out << "CheckAdEmpty " << tttt << std::endl;
    #endif
}

void Chunk::Display(){
    for (std::set<int>::iterator it = onDraw.begin(); it != onDraw.end(); it++){
        block[*it].Display();
    }
    #ifdef Fuck
        std::cout << onDraw.size() << std::endl;
    #endif
}

void Chunk::Load(int x, int z){
    
    Position = glm::vec2(x, z);
    
    char a[10], b[10];
    itoa(x, a, 10);
    itoa(z, b, 10);
    std::string FileName = ChunkFilePath + a + "_" + b;
    FILE *fp = NULL;
    if ((fp = fopen(FileName.data(), "r")) == NULL){
        randomProduce();
        WrittenBack();
    }
    else {
        fgets(temp, FileSize, fp);
        char *p = temp;
        Block *pb = block;
        for (int i = 0, j, temp; i < blockNum; i++, pb++){
            while (*p == ' ') p++;
            for (j = 0;*p >= '0' && *p <= '9'; p++) {
                j = j * 10 + *p - '0';
            }
            pb->bt = BlockType(j);
        }
    }
    fclose(fp);
    for (int i = 0; i < 4; i++){
        AdjChunk[i] = NULL;
    }
    CheckAdEmpty();
}

void Chunk::WrittenBack(){
    #ifdef Fuck
        std::cout << "WrittenBack" << Position.x << " " << Position.y << std::endl;
    #endif
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

enum ChunkEvent{
    ceWrittenBack,
    ceLoad,
    ceCheckAdEmpty,
    ceEventNum
};


class ChunkManager{
public:
    std::map<std::PII, int> ChunkPosMap;
    static const int num = 500;
    int r;
    bool loaded[num];
    Chunk chunk[num];
    // Event System
    int EventTimeHead, EventTimeTail;
    std::queue<std::queue<int> > EventQue; // Event: <Time, ChunkID, Type, ...>
    bool EventMap[num][ceEventNum];
    
public:
    ChunkManager(int radius = 5){
        r = radius;
        for (int i = 0; i < num; i++){
            loaded[i] = false;
            for (int j = 0; j < ceEventNum; j++){
                EventMap[i][j] = false;
            }
        }
        ChunkPosMap.clear();
        
        // Event System init
        EventTimeHead = EventTimeTail = 0;
        while (!EventQue.empty()) EventQue.pop();
    }
    ~ChunkManager(){
        for (int i = 0; i < num; i++){
            if (loaded[i]){
                chunk[i].WrittenBack();
            }
        }
    }
    
    //void Load();
    void CheckPos(glm::vec3 PlayerPos);
    void Display();
    void RunEvent();
    void PushEvent(int time, int chunkId, ChunkEvent type, int x = 0, int y = 0);
};

#ifdef boooom
#include <iostream>
#include <fstream>
#endif

#define Square(a) ((a) * (a))
#define SquareSum(a,b) (Square(a) + Square(b))
#define Modu(p) SquareSum(p.first,p.second)
void ChunkManager::CheckPos(glm::vec3 PlayerPos){
    // Time in Event System
    EventTimeHead++;
    if (EventTimeTail < EventTimeHead) EventTimeTail = EventTimeHead;
    
    // static int lastx = 0;
    // static int lasty = 0;
    static int firstCheck = 1;
    static int tot = 0;
    static std::PII disp[num];
    int x = PlayerPos.x / Chunk::xLength;
    if (PlayerPos.x < 0) x--;
    int y = PlayerPos.z / Chunk::zLength;
    if (PlayerPos.z < 0) y--;
    if (firstCheck){
        for (int i = -r; i <= r; i++){
            for (int j = -r; j <= r; j++){
                if (SquareSum(i, j)<= r * r){
                    disp[tot++] = std::make_pair(i, j);
                }
            }
        }
        for (int i = 0; i < tot; i++){
            for (int j = i + 1; j < tot; j++){
                if (Modu(disp[i]) > Modu(disp[j])){
                    std::swap(disp[i], disp[j]);
                }
            }
        }
        firstCheck = 0;
    }
    
    // main check
    //if (lastx != x || lasty != y){
        for (int i = 0; i < num; i++)
        if (loaded[i]){
            int dx = chunk[i].Position.x - x;
            int dy = chunk[i].Position.y - y;
            if (dx * dx + dy * dy > r * r){
                //chunk[i].WrittenBack();
                PushEvent(1, i, ceWrittenBack);
            }
        }
        for (int i, j, k = 0, p = 0; p < tot; p++){
            i = disp[p].first + x;
            j = disp[p].second + y;
            if (ChunkPosMap.find(std::make_pair(i, j)) == ChunkPosMap.end()){
                for (; k < num && loaded[k]; k++);
                if (k < num){
                    //chunk[k].Load(x + i, y + j);
                    PushEvent(2, k, ceLoad, i, j);
                    //EventLoadPos.push(std::make_pair(x + i, y + i),);
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
            int change = 0;
            for (int d = 0; d < 4; d++){
                tox = cx + dx[d];
                toy = cy + dy[d];
                Chunk *temp = chunk[i].AdjChunk[d];
                if (ChunkPosMap.find(std::make_pair(tox, toy)) != ChunkPosMap.end()){
                    k = ChunkPosMap[std::make_pair(tox, toy)];
                    chunk[i].AdjChunk[d] = chunk + k;
                }
                else {
                    chunk[i].AdjChunk[d] = NULL;
                }
                if (temp != chunk[i].AdjChunk[d]){
                    change = 1;
                }
            }
            if (change){
                //chunk[i].CheckAdEmpty();
                PushEvent(2, i, ceCheckAdEmpty);
            }
        }
        
        #ifdef boooom
            std::ofstream out;
            out.open("er.txt", std::ios::app);
            out << "loaded" << std::endl;
            for (int i = 0; i < num; i++)if (loaded[i]){
                out << i << " " << chunk[i].Position.x << " " << chunk[i].Position.y << std::endl;
            }
        #endif
    //}
    
    
    RunEvent();
    
    #ifdef nodef
        std::ofstream out;
        out.open("er.txt", std::ios::app);
        out << "pppp" << std::endl;
        out.close();
    #endif
    
    // lastx = x;
    // lasty = y;
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

void ChunkManager::RunEvent(){
    /*enum ChunkEvent{
        ceWrittenBack,
        ceLoad,
        ceCheckAdEmpty
    };*/
    std::queue<int> p;
    int a[5] = {0};//time id type
    for (;!EventQue.empty() && (p = EventQue.front()).front() <= EventTimeHead; EventQue.pop()){
        for (int i = 0; i < 5 && !p.empty(); i++){
            a[i] = p.front();
            p.pop();
        }
        int id = a[1];
        int type = a[2];
        if (type == ceWrittenBack){
            loaded[id] = false;
            ChunkPosMap.erase(std::make_pair(chunk[id].Position.x, chunk[id].Position.y));
            chunk[id].onDraw.clear();
            chunk[id].WrittenBack();
        }
        else if (type == ceCheckAdEmpty){
            chunk[id].CheckAdEmpty();
        }
        else if (type == ceLoad){
            loaded[id] = true;
            ChunkPosMap[std::make_pair(a[3], a[4])] = id;
            chunk[id].Load(a[3], a[4]);
        }
        EventMap[id][type] = false;
        #ifdef boooom
        std::ofstream out;
        out.open("er.txt", std::ios::app);
        out <<"run " << a[0] << " " << chunk[a[1]].Position.x << " " << chunk[a[1]].Position.y << " " << int(a[2]) << std::endl;
        out.close();
        #endif
    }
}
void ChunkManager::PushEvent(int time, int chunkId, ChunkEvent type, int x, int y){
    if (EventMap[chunkId][type]){
        return;
    }
    EventMap[chunkId][type] = true;
    time = EventTimeTail += time;
    std::queue<int> a;
    while (!a.empty()) a.pop();
    a.push(time);
    a.push(chunkId);
    a.push(int(type));
    if (type == ceLoad){
        a.push(x);
        a.push(y);
    }
    EventQue.push(a);
    #ifdef boooom
    std::ofstream out;
    out.open("er.txt", std::ios::app);
    out <<"push " << time << " " << chunkId << " " << int(type) << std::endl;
    out.close();
    #endif
}

// End class ChunkManager
#endif
