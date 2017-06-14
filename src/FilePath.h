#ifndef __FILEPATH_H__
#define __FILEPATH_H__
    
#include <string>

// Main Directories
const std::string dataPath = "data/";
const std::string ChunkFilePath = dataPath + "Chunk/";
const std::string TexturePath = dataPath + "Texture/";
const std::string LightPath = dataPath + "Light/";
const std::string ModelPath = dataPath + "Model/";

// Light Path
const std::string PointLightFilePath = LightPath + "pl.txt";
const std::string LightSunPath = LightPath + "sun.txt";

//Model Path
const std::string ModelBallPath = ModelPath + "ball.obj";

//Texture
const std::string BlockTexture = TexturePath + "blocks/";

const std::string TempPath = TexturePath;

#endif