#include "Player.h"
#include "obj.h"
#include "Shader.h"
#include <SOIL.h>
#include "FilePath.h"

//GLfloat player_vertices[10000];

void Player::Load()
{
#ifdef boooom
	std::ofstream out;
	out.open("er.txt");
	out << "Load begin" << std::endl;
#endif
	shader = Shader("cube.vs", "cube.frag");
	// Bind verteces array
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
#ifdef boooom
	out << "test point 1" << std::endl;
#endif

	std::vector<glm::vec3> positions, normals;
	std::vector<glm::vec2> textures;

	LoadOBJ("Model/cone.obj", positions, textures, normals);

	int size_of_vertices = 0;
	for (int i = 0; i < positions.size(); ++i) {

		player_vertices[size_of_vertices++] = positions[i].x;
		player_vertices[size_of_vertices++] = positions[i].y;
		player_vertices[size_of_vertices++] = positions[i].z;
		player_vertices[size_of_vertices++] = normals[i].x;
		player_vertices[size_of_vertices++] = normals[i].y;
		player_vertices[size_of_vertices++] = normals[i].z;
		player_vertices[size_of_vertices++] = textures[i].x;
		player_vertices[size_of_vertices++] = textures[i].y;
	}

	size_of_vertices *= sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size_of_vertices, player_vertices, GL_STATIC_DRAW);

#ifdef boooom
	out << "test point 1" << std::endl;
#endif
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Texture load
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);
	glGenTextures(1, &emissionMap);
	int width, height;
	unsigned char* image;
	// Diffuse map
	image = SOIL_load_image((TempPath + "container2.png").data(), &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Specular map
	image = SOIL_load_image((TempPath + "container2_specular.png").data(), &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Set texture units
	shader.Use();
	glUniform1i(glGetUniformLocation(shader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shader.Program, "material.specular"), 1);
#ifdef boooom

	out << "Load end" << std::endl;
	out.close();
#endif

}

void Player::Display() {
	//GLint modelLoc = glGetUniformLocation(Player::shader.Program, "model");
	///*glm::vec3 WorldPos = glm::vec3(Position.x * xLength, 0.0f, Position.y * zLength);
	//glm::vec3 LocPos;

	//	LocPos = glm::vec3(*it / (yLength * zLength),*/
	//		*it & yLength - 1,
	//		*it / yLength & zLength - 1);

	//	glm::mat4 model = glm::translate(glm::mat4(), WorldPos + LocPos);
	//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//	// Draw a cube
	//	glDrawArrays(GL_TRIANGLES, 0, 1000);

#ifdef Fuck
	std::cout << onDraw.size() << std::endl;
#endif
}