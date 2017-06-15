#ifndef __MATERIA_CC__
#define __MATERIA_CC__

//#define boooom
#ifdef boooom
#include <fstream>
#endif

#include "Materia.h"

int Materia::UsingIndices[MateriaTypeNum][2];
Materia::Image Materia::image[MateriaTypeNum][2];
GLuint Materia::TextureMap[Materia::MAX_NUM];
float Materia::lastUsingTime[Materia::MAX_NUM] = {0.0f};

Materia::~Materia(){
    for (int i = 0; i < MateriaTypeNum; i++){
        for (int j = 0;j < 2; j++){
            SOIL_free_image_data(image[i][j].image);
        }
    }
}

void Materia::Load(){
    // Load textures from disk, first they are justed loaded in main memory but not pushed to GPU
    for (int i = 0; i < MateriaTypeNum; i++){
        for (int j = 0; j < 2; j++){
            image[i][j].image = SOIL_load_image((TexturePath + MateriaFile[i][j]).data(), &(image[i][j].width), &(image[i][j].height), 0, SOIL_LOAD_RGB);
            UsingIndices[i][j] = -1;
        }
    }
    
    glGenTextures(MAX_NUM, TextureMap);
    for (int i = 0; i < MAX_NUM; i++){
        lastUsingTime[i] = -1.0f;
    }
    
}


void Materia::Apply(MateriaType mt[], int res[][2], int num){
    #ifdef boooom
        std::ofstream out;
        out.open("er.txt", std::ios::app);
        out << "Materia :: Apply" << std::endl;
    #endif
    if (num > MAX_NUM / 2){
        // too many textures to apply at one time, failed
        return;
    }
    for (int ii = 0, i; ii < num; ii++){
        i = mt[ii];
        for (int j = 0; j < 2; j++){
            int &k = UsingIndices[i][j];
            if (k == -1){
                // find the earliest texture map to use
                int min_pos = 0;
                for (int t = 1; t < MAX_NUM; t++){
                    if (lastUsingTime[min_pos] > lastUsingTime[t]){
                        min_pos = t;
                    }
                }
                for (int t = 0; t < MateriaTypeNum; t++){
                    for (int s = 0; s < 2; s++){
                        if (UsingIndices[t][s] == min_pos){
                            UsingIndices[t][s] = -1;
                            break;
                        }
                    }
                }
                k = min_pos;
                
                // deliver image[i][j] to texture map
                glBindTexture(GL_TEXTURE_2D, TextureMap[k]);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[i][j].width, image[i][j].height, 0, GL_RGB, GL_UNSIGNED_BYTE, image[i][j].image);
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            glActiveTexture(GL_TEXTURE0 + k);
            glBindTexture(GL_TEXTURE_2D, TextureMap[k]);
            res[ii][j] = k;
            #ifdef boooom
                out << k << " " << TextureMap[k] << std::endl;
            #endif
            lastUsingTime[k] = Model::lastFrame + Model::deltaTime;
        }
    }
    #ifdef boooom
        out.close();
    #endif
}

#endif