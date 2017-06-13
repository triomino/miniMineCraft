#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <glm/glm.hpp>
#include "Shader.h"

class Player{
private:
    glm::vec3 Postion;
	Shader shader;
	GLuint VBO, VAO;
	GLfloat player_vertices[10000];
	// texture map
	GLuint diffuseMap, specularMap, emissionMap;

public:
    glm::vec3 getPos(){
        return Postion;
    }
	void Load();
	void Display();
};

#endif