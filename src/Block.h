#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <glm/glm.hpp>
#include "Materia.h"

enum BlockType{
    Empty,
    Grass,
    Dirt,
    Cloud,
    Water,
    Pumpkin,
    BlockTypeNum
};

const MateriaType blockMateria[BlockTypeNum][3] = {
    {MateriaTypeNum, MateriaTypeNum, MateriaTypeNum},
    {mtGrassTop, mtGrassSide, mtDirt},
    {mtDirt, mtDirt, mtDirt},
    {mtCloud, mtCloud, mtCloud},
    {mtWater, mtWater, mtWater},
    {mtPumpkinTop, mtPumpkinSide, mtPumpkinBottom}
};


class Block{
friend class Chunk;
friend class ChunkManager;
private:
    BlockType bt, AdjType[6];
public:
    Block(){
        bt = Empty;
    }
    ~Block(){
        
    }
    /*void MateriaApply(GLuint Program){
        static BlockType lastBT = Empty;
        if (bt != Empty){
            if (bt != lastBT){
                MateriaType mt[3] = {blockMateria[bt][0], blockMateria[bt][1], blockMateria[bt][2]};
                int res[3][2] = {0};
                Materia::Apply(mt, res, 3);
                for (int i = 0; i < 3; i++){
                    char num[3];
                    itoa(i, num, 10);
                    std::string temp = std::string("material[") + num + "].", name[2] = {"diffuse", "specular"};
                    
                    for (int j = 0; j < 2; j++){
                        glUniform1i(glGetUniformLocation(Program, (temp + name[j]).data()), res[i][j]);
                    }
                    glUniform1f(glGetUniformLocation(Program, (temp +  "shininess").data()), 32.0f);
                }
                lastBT = bt;
            }
        }
    }*/
};

#endif