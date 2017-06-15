#ifndef __LIGHT_CC__
#define __LIGHT_CC__

#include "Light.h"
#include "obj.h"

DirLight Light::dl[max_num];
PointLight Light::pl[max_num];  
float Light::SunAngle = 0.0f;
Shader Light::SunShader;
GLuint Light::SunVAO, Light::SunVerticesNum;

void DirLight::Apply(GLuint Program){
    // Directional light
    glUniform3f(glGetUniformLocation(Program, "dirLight.direction"), direction.x, direction.y, direction.z);
    //glUniform3f(glGetUniformLocation(Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(Program, "dirLight.ambient"),ambient.x, ambient.y, ambient.z);
    glUniform3f(glGetUniformLocation(Program, "dirLight.diffuse"), diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(glGetUniformLocation(Program, "dirLight.specular"), specular.x, specular.y, specular.z);
}

void PointLight::Apply(GLuint Program, GLuint lab){
    char temp[10];
    itoa(lab, temp, 10);
    std::string s("pointLights[");
    s = s + temp + "].";
    if (FollowCamera) {
        position = Model::getCamera().Position;
    }
    glUniform3f(glGetUniformLocation(Program, (s +"position").data()), position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(Program, (s +"ambient").data()),ambient.x, ambient.y, ambient.z);
    glUniform3f(glGetUniformLocation(Program, (s +"diffuse").data()), diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(glGetUniformLocation(Program, (s +"specular").data()), specular.x, specular.y, specular.z);
    glUniform1f(glGetUniformLocation(Program, (s +"constant").data()), constant);
    glUniform1f(glGetUniformLocation(Program, (s +"linear").data()), linear);
    glUniform1f(glGetUniformLocation(Program, (s +"quadratic").data()), quadratic);
    
    glUniform1i(glGetUniformLocation(Program, (s + "Enable").data()), Enable);
}

void Light::Load(){
    static bool LightLoaded = false;
    if (!LightLoaded){
        LoadPointLights();
        LoadSun();
        
        LightLoaded = true;
    }
    
}

void Light::LoadPointLights(){
    std::ifstream in;
    in.open(PointLightFilePath.data());
    std::string s;
    int i = 0;
    while (!in.eof() && i < max_num){
        in >> s;
        while (s != "fixed" && s != "Camera" && !in.eof()){
            in >> s;
        }
        if (s == "fixed"){
           pl[i].FollowCamera = false;
           in >> pl[i].position.x >> pl[i].position.y >> pl[i].position.z;
        }
        else if (s == "Camera"){
            pl[i].FollowCamera = true;
        }
        else {
            break;
        }
        in >> pl[i].ambient.x >> pl[i].ambient.y >> pl[i].ambient.z;
        in >> pl[i].diffuse.x >> pl[i].diffuse.y >> pl[i].diffuse.z;
        in >> pl[i].specular.x >> pl[i].specular.y >> pl[i].specular.z;
        in >> pl[i].constant >> pl[i].linear >> pl[i].quadratic;
        
        pl[i].Enable = true;
        i++;
    }
    in.close();
}

void Light::LoadSun(){
    SunAngle = 0.0f;
    SunShader = Shader("src/sun.vs", "src/sun.frag");
    std::ifstream in;
    in.open(LightSunPath.data());
    /*in >> dl[0].ambient.x >> dl[0].ambient.y >> dl[0].ambient.z;
    in >> dl[0].diffuse.x >> dl[0].diffuse.y >> dl[0].diffuse.z;
    in >> dl[0].specular.x >> dl[0].specular.y >> dl[0].specular.z;
    */
    in.close();
    
    LoadOBJ(ModelBallPath, SunVAO, SunVerticesNum);
    
}


void Light::Apply(GLuint Program){
    const Camera &camera = Model::getCamera();
    
    dl[0].Apply(Program);

    for (int i = 0; i < max_num; i++){
        pl[i].Apply(Program, i);
    }
    // SpotLight
    glUniform3f(glGetUniformLocation(Program, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
    glUniform3f(glGetUniformLocation(Program, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
    glUniform3f(glGetUniformLocation(Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(Program, "spotLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(Program, "spotLight.linear"), 0.09);
    glUniform1f(glGetUniformLocation(Program, "spotLight.quadratic"), 0.032);
    glUniform1f(glGetUniformLocation(Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
    glUniform1f(glGetUniformLocation(Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
}

const float MAX_SUN_AMBIENT = 0.2;
const float MIN_SUN_AMBIENT = 0.05;
const float MAX_SUN_DIFFUSE = 0.7;
const float MIN_SUN_DIFFUSE = 0.1;
const float MAX_SUN_SPECULAR = 0.4;
const float MIN_SUN_SPECULAR = 0.1;

void Light::SunMove(){
    const Camera &camera = Model::getCamera();
    SunAngle += SunAngularSpeed * Model::deltaTime;
    dl[0].direction = glm::vec3(-cos(glm::radians(SunAngle)), -sin(glm::radians(SunAngle)), 0.0f);
    
    if (SunAngle > 360.0f){
        SunAngle -= 360.f;
    }
    float x, y, z;
    if (SunAngle > 0.0f && SunAngle < 180.0f){
        x = MAX_SUN_AMBIENT - fabs(90.0f - SunAngle) / 90.0f * (MAX_SUN_AMBIENT - MIN_SUN_AMBIENT);
        y = MAX_SUN_DIFFUSE - fabs(90.0f - SunAngle) / 90.0f * (MAX_SUN_DIFFUSE - MIN_SUN_DIFFUSE);
        z = MAX_SUN_SPECULAR - fabs(90.0f - SunAngle) / 90.0f * (MAX_SUN_SPECULAR - MIN_SUN_SPECULAR);
    }
    else {
        x = MIN_SUN_AMBIENT;
        y = MIN_SUN_DIFFUSE;
        z = MIN_SUN_SPECULAR;
    }
    dl[0].ambient = glm::vec3(x, x, x);
    dl[0].diffuse = glm::vec3(y, y, y);
    dl[0].specular = glm::vec3(z, z, z);
}

void Light::Display(){
    glBindVertexArray(SunVAO);
    const Camera &camera = Model::getCamera();
    
    SunShader.Use();
    GLuint Program = SunShader.Program;
    GLuint modelLoc = glGetUniformLocation(Program, "model");
    GLuint viewLoc  = glGetUniformLocation(Program, "view");
    GLuint projLoc  = glGetUniformLocation(Program, "projection");
    // Calculate matrices
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)camera.WIDTH / (GLfloat)camera.HEIGHT, 0.1f, 100.0f);
    // Set matrices
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    glm::mat4 model = glm::mat4();
    model = glm::translate(model, glm::vec3(camera.Position.x, Chunk::yHalf, camera.Position.z));
    model = glm::rotate(model,  glm::radians(SunAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(SunDistance, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(SunSize, SunSize, SunSize));
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, SunVerticesNum);
    
    
    glBindVertexArray(0);
}

#endif