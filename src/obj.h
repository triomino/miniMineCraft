#ifndef __OBJ_H__
#define __OBJ_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void LoadOBJ(const std::string filename, GLuint &VAO, GLuint &size);

void LoadOBJ(const std::string filename, GLuint &VAO, GLuint &size, std::vector<glm::vec3> &vertices);

void LoadOBJ(const std::string filename, 
	std::vector<glm::vec3> &positions, 
	std::vector<glm::vec2> &textures, 
	std::vector<glm::vec3> &normals);
    
#endif