#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <stdio.h>
#include <string>
#include <glm/glm.hpp>
#include "FilePath.h"
#include "Model.h"

struct DirLight {
    glm::vec3 direction;
	
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    bool Enable;
    
    void Load(){
        
    }
    
    void Apply(GLuint Program);
    
};

struct PointLight {
    glm::vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    bool Enable;
    bool FollowCamera;
    
    void Apply(GLuint Program, GLuint lab);
};


class Light{
public:
    static const int max_num = 4;
    static const float SunAngularSpeed = 15.0f;
    static const float SunDistance = 80.0f;
    static const float SunSize = 1.0f;
private:
    static DirLight dl[max_num];
    static PointLight pl[max_num];
    
    // sun
    static float SunAngle;
    static Shader SunShader;
    static GLuint SunVAO, SunVerticesNum;
    
    static void LoadSun();
    static void LoadPointLights();
    
public:
    static void Load();
    static void Apply(GLuint Program);
    static void SunMove();
    static void Display();
};

#endif