#ifndef __MATERIA_H__
#define __MATERIA_H__

#include <string>
enum MateriaType{
    mtContainer,
    mtDirt,
    mtGrassSide,
    mtGrassTop,
    mtSteve,
    mtCloud,
    mtWater,
    mtPumpkinTop,
    mtPumpkinSide,
    mtPumpkinBottom,
    MateriaTypeNum
};

// diffuse map and specular map
const std::string MateriaFile[MateriaTypeNum][2] = {
    {"container2.png","container2_specular.png"},
    {"blocks/dirt.png", "blocks/dirt.png"},
    {"blocks/grass_side.png", "blocks/grass_side.png"},
    {"blocks/grass_top.png", "blocks/grass_top.png"},
    {"Steve.png", "Steve.png"},
    {"blocks/cloud.png", "blocks/cloud.png"},
    {"", ""},
    {"blocks/pumpkin_top.png", "blocks/pumpkin_top.png"},
    {"blocks/pumpkin_face_on.png", "blocks/pumpkin_face_on.png"},
    {"blocks/pumpkin_top.png", "blocks/pumpkin_top.png"}
};

#include "Model.h"

class Materia{
public:
    struct Image{
        unsigned char *image;
        int width, height;
    };
    const static int MAX_NUM = 16;
private:
    static int UsingIndices[MateriaTypeNum][2];
    static Image image[MateriaTypeNum][2];
    static GLuint TextureMap[MAX_NUM];
    static float lastUsingTime[MAX_NUM];
    
public:
    ~Materia();
    static void Load();
    static void Apply(MateriaType mt[], int res[][2], int num);
};

#endif
