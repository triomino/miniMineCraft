#ifndef __PLAYER_CC__
#define __PLAYER_CC__

#include "Player.h"
#include "Model.h"

#define boooom
#ifdef boooom
#include <fstream>
#endif

Shader Player::shader;
GLuint Player::VAO, Player::verticesNum;
    

void Player::Load(){
    shader = Shader("src/cube.vs", "src/cube.frag");
    LoadOBJ(ModelPath + "steve.obj", VAO, verticesNum);
    #ifdef boooom
        std::ofstream out;
        out.open("er.txt");
        out << "-------------------------" << VAO << " " << verticesNum << std::endl;
        out.close();
     #endif 
}

void Player::Display(){
    #ifdef boooom
        std::ofstream out;
        out.open("er.txt");
        out << "-------------------------" << VAO << " " << verticesNum << std::endl;
        
    #endif 
    shader.Use();
    glBindVertexArray(VAO);
    const Camera &camera = Model::getCamera();
        
    GLint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
    glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
    
    Light::Apply(shader.Program);
    
    // Create camera transformations
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)camera.WIDTH / (GLfloat)camera.HEIGHT, 0.1f, 100.0f);
    glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0.0f, 80.0f, 0.0f));
    // Get the uniform locations
    GLint viewLoc  = glGetUniformLocation(shader.Program, "view");
    GLint projLoc  = glGetUniformLocation(shader.Program, "projection");
    GLint modelLoc  = glGetUniformLocation(shader.Program, "model");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    MateriaType mt[1] = {mtSteve};
    int TextureMap[1][2];
    Materia::Apply(mt, TextureMap, 1);
    
    std::string temp = "material[0].";
    
    glUniform1i(glGetUniformLocation(shader.Program, (temp + "diffuse").data()), TextureMap[0][0]);
    glUniform1i(glGetUniformLocation(shader.Program, (temp + "specular").data()), TextureMap[0][1]);

    glUniform1f(glGetUniformLocation(shader.Program, (temp +  "shininess").data()), 32.0f);
    
    glUniform1i(glGetUniformLocation(shader.Program, "faceFlag"), 0);
    
    
    glDrawArrays(GL_TRIANGLES, 0, verticesNum);
    
    glBindVertexArray(0);
    
    #ifdef boooom
        out.close();
    
    #endif
}
#endif