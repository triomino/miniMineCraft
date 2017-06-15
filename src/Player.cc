#ifndef __PLAYER_CC__
#define __PLAYER_CC__

#include <fstream>
#include <algorithm>
#include "Player.h"
#include "Model.h"

//#define boooom

Shader Player::shader;
GLuint Player::VAO, Player::verticesNum;
glm::vec3 Player::offset = glm::vec3(0.0f, -0.2f, 0.0f);

GLfloat Player::MovementSpeed;
GLfloat Player::shininess;
    

void Player::Load(){
    shader = Shader("src/cube.vs", "src/cube.frag");
    
    std::vector<glm::vec3> temp;
    temp.clear();
    LoadOBJ(ModelPath + "steve.obj", VAO, verticesNum, temp);    
    ClVerticesNum = temp.size() * 3;
    
    vertices = new float[ClVerticesNum];
    int i = 0;
    for (std::vector<glm::vec3>::iterator it = temp.begin(); it != temp.end(); it++){
        vertices[i++] = it->x;
        vertices[i++] = it->y;
        vertices[i++] = it->z;
    }    
    
    /*for (std::vector<glm::vec3>::iterator it = vertices.begin(); it != vertices.end(); it++){
        vertices.erase(it);
    }*/
    //vertices.push_back();
    
    std::ifstream in;
    in.open((PlayerPath + "player.txt").data());
    float x, y, z;
    in >> x >> y >> z;
    Position = glm::vec3(x, y, z);
    in >> y >> x;
    shininess = x;
    MovementSpeed = y;
    in.close();
    
    #ifdef boooom
        std::ofstream out;
        out.open("d2.txt");
        out << ClVerticesNum << std::endl;
        out << Position.x << Position.y << Position.z << std::endl;
        out.close();
    #endif
    
}

Player::~Player(){
    delete []vertices;
    std::ofstream out;
    out.open((PlayerPath + "player.txt").data());
    out << Position.x << " " << Position.y + 0.5 << " " << Position.z << std::endl;
    out << MovementSpeed << " " << shininess << std::endl;
    out.close();
}

void Player::Display(){
    shader.Use();
    glBindVertexArray(VAO);
    const Camera &camera = Model::getCamera();
        
    GLint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
    glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
    
    Light::Apply(shader.Program);
    
    // Create camera transformations
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)camera.WIDTH / (GLfloat)camera.HEIGHT, 0.1f, 100.0f);
    // Get the uniform locations
    GLint viewLoc  = glGetUniformLocation(shader.Program, "view");
    GLint projLoc  = glGetUniformLocation(shader.Program, "projection");
    GLint modelLoc  = glGetUniformLocation(shader.Program, "model");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
        //calculate model matrix
        glm::vec3 trans = Position + offset;
        glm::mat4 model = glm::translate(glm::mat4(), trans);
    
        model = glm::rotate(model, -glm::radians(Yaw), Model::getCamera().WorldUp);
        model = glm::scale(model, glm::vec3(ScaleSize, ScaleSize, ScaleSize));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
   
    // apply material
    MateriaType mt[1] = {mtSteve};
    int TextureMap[1][2];
    Materia::Apply(mt, TextureMap, 1);
    
    std::string temp = "material[0].";
    
    glUniform1i(glGetUniformLocation(shader.Program, (temp + "diffuse").data()), TextureMap[0][0]);
    glUniform1i(glGetUniformLocation(shader.Program, (temp + "specular").data()), TextureMap[0][1]);

    glUniform1f(glGetUniformLocation(shader.Program, (temp +  "shininess").data()), shininess);
    
    glUniform1i(glGetUniformLocation(shader.Program, "faceFlag"), 0);
    
    if (camera.camera_mode == FirstPerspective){
        //glDrawArrays(GL_TRIANGLES, 0, verticesNum /);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, verticesNum);
    }
    
    glBindVertexArray(0);
}

//Control: Movement
const GLfloat MAX_FALL_SPEED = 100000.0f;
const GLfloat JUMP_UP_SPEED = 5.0f;
const GLfloat JUMP_HRZ_SPEED = 2.0f;
const GLfloat GRAVITY = 9.8f;

const GLfloat HRZ_BEG_SPEED = 4.317;
const GLfloat HRZ_ACCELERATOR = 10.0f;
const GLfloat MAX_HRZ_SPEED = 5.982;

const GLfloat EPS = 1e-2;

const GLfloat ENG_REDUCE = 0.5;
void Player::ProcessKeyboard(const bool *keys, GLfloat deltaTime){
    #ifdef boooom
        std::ofstream out;
        out.open("debug.txt", std::ios::app);
        out << " press" << std::endl;
    #endif
    
    calPlayerModelMatrix();
    
    static glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    
    glm::vec3 tFront = glm::normalize(glm::cross(Model::getCamera().WorldUp, Right));
    if (onGround()){
        bool pressed = false;
        if (keys[GLFW_KEY_W]) (velocity = tFront), pressed = true;
        if (keys[GLFW_KEY_S]) (velocity += -tFront), pressed = true;
        if (keys[GLFW_KEY_A]) (velocity += -Right), pressed = true;
        if (keys[GLFW_KEY_D]) (velocity += Right), pressed = true;
        velocity = glm::normalize(velocity);
        
        if (pressed){
            if (keys[GLFW_KEY_SPACE]){
                velocity *= JUMP_HRZ_SPEED;
                velocity.y = JUMP_UP_SPEED;
            }
            else {
                velocity *= HRZ_BEG_SPEED;
            }
        }
        else {
            velocity *= 0.01;
        }
        #ifdef boooom
            out << "on ground" << std::endl;
        #endif
    }
    else {
        bool pressed = false;
        velocity.x = velocity.z = 0.0f;
        if (keys[GLFW_KEY_W]) (velocity += tFront * JUMP_HRZ_SPEED), pressed = true;
        if (keys[GLFW_KEY_S]) (velocity += -tFront * JUMP_HRZ_SPEED), pressed = true;
        if (keys[GLFW_KEY_A]) (velocity += -Right * JUMP_HRZ_SPEED), pressed = true;
        if (keys[GLFW_KEY_D]) (velocity += Right * JUMP_HRZ_SPEED), pressed = true;
        
        velocity.y -= deltaTime * GRAVITY;
        if (velocity.y <= -MAX_FALL_SPEED) {
            velocity.y = -MAX_FALL_SPEED;
        }
    }
    
    #ifdef  boooom
        //out << "boooom" << std::endl;
    #endif
    
    glm::vec3 displacement = velocity * deltaTime;
    int tot = 0;
    while (collision(0.0f, displacement.y, 0.0f) && (tot++ < 3)){
        velocity.y *= ENG_REDUCE;
        displacement.y *= ENG_REDUCE;
    }
    while (collision(displacement.x , 0.0f, 0.0f) && (tot++ < 6)){
        velocity.x *= ENG_REDUCE;
        displacement.x *= ENG_REDUCE;
    }

    while (collision(0.0f, 0.0f, displacement.z)&& (tot++ < 9)){
        velocity.z *= ENG_REDUCE;
        displacement.z *= ENG_REDUCE;
    }

    if (!collision(displacement)){
        Position += displacement;
    }
    else {
        if (!collision(0.0f, displacement.y, 0.0f)){
            Position += glm::vec3(0.0f, displacement.y, 0.0f);
        }
    }
    
    #ifdef boooom
        
           out << "v " << velocity.x << " " << velocity.y << " " << velocity.z << std::endl;
            out << "dis " << displacement.x << " " << displacement.y << " " << displacement.z << std::endl;
          out <<"pos" << Position.x << " " << Position.y << " " << Position.z << std::endl;
        out.close();
    #endif 
}

void Player::setYaw(GLfloat yaw){
    float temp = Yaw;
    Yaw = yaw;
    calPlayerModelMatrix();
    if (collision(0.0f, 0.0f, 0.0f)){
        Yaw = temp;
    }
}

bool Player::onGround(){
    return collision(0.0f, -0.03f, 0.0f);
}

bool Player::collision(float x, float y, float z){
    return collision(glm::vec3(x, y, z));
}

bool Player::collision(glm::vec3 disp){
    #ifdef boooom
    std::ofstream out ;
    out.open("d2.txt", std::ios::app);
    #endif
    for (int i = 0; i < ClVerticesNum; i += 3){
        #ifdef boooom
            out << i << std::endl;
        #endif
        glm::vec4 p = PlayerModelMatrix * glm::vec4(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
        
        #ifdef boooom
            out << i << " " <<disp.x + p.x << " " << disp.x + p.y << " " << disp.x + p.z << std::endl;
        #endif
        if (Model::hasCube(disp.x + p.x, disp.y + p.y, disp.z + p.z)){
          return true;
        }
        #ifdef boooom
            out << i << std::endl;
        #endif
    }
    
    //glm::vec4 p = PlayerModelMatrix * glm::vec4(vertices[0], vertices[1], vertices[2], 1.0f);
    #ifdef boooom
        out << p.x << " " << p.y << " " << p.z << std::endl;
    #endif
    //glm::vec3 t = glm::vec3(disp.x, disp.y, disp.z);
    
    //bool x = Model::hasCube(t +  disp);
    //if (x){
    // return true;
    //}
    #ifdef boooom
        out << " out " << std::endl;
        out.close();
    #endif
    
    bool u = Model::hasCube(Position + disp);
    return u;
}

void Player::calPlayerModelMatrix(){
    glm::vec3 trans = Position + offset;
    glm::mat4 model = glm::translate(glm::mat4(), trans);

    model = glm::rotate(model, -glm::radians(Yaw), Model::getCamera().WorldUp);
    model = glm::scale(model, glm::vec3(ScaleSize, ScaleSize, ScaleSize));
    PlayerModelMatrix = model;
    //PlayerModelMatrix = model;
}

#endif