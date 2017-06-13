#pragma once
// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void LoadOBJ(std::string filename,
	std::vector<glm::vec3> &positions,
	std::vector<glm::vec2> &textures,
	std::vector<glm::vec3> &normals);


