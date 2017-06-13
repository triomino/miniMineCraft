#ifndef __CHUNK_CC__
#define __CHUNK_CC__

#include "obj.h"
#include "Model.h"
//#define boooom
#ifdef boooom
#include <iostream>
#include <fstream>
#include <string>
int tttt = 0;
#endif


char Chunk::temp[Chunk::FileSize] = "";

//class Chunk methods
void Chunk::randomProduce(){
    Block *pb = block;
    //int k = yHalf + RandomGetInt(10);
    int k = yHalf;
    for (int i = 0; i < xLength; i++){
        for (int j = 0, l; j < zLength; j++){
            for (l = 0; l < k; l++){
                (pb++)->bt = Soil;
            }
            (pb++)->bt = Grass;
            for (l = k + 1; l < yLength; l++){
                (pb++)->bt = Empty;
            }
            //k = yHalf + RandomGetInt(10);
            int temp = Random::GetInt(-10, 11);
            if (temp < -8) k--;
            if (temp > 8) k++;
            if (k < yHalf - 3) k = yHalf - 3;
            if (k > yHalf + 3) k = yHalf + 3;
        }
    }
}


void Chunk::CheckAdEmpty(){
    const int dx[] = {0, 0, 0, 0, -1, 1};
    const int dz[] = {0, 0, -1, 1, 0, 0};
    const int dy[] = {-1, 1, 0, 0, 0, 0};
    const int is = zLength * yLength;
    const int js = yLength;
    int cnt = 0;
    Block *pb = block;
    Block *pad;
    onDraw.clear();
    for (int i = 0, j, k, d; i < xLength; i++){
        for (j = 0; j < zLength; j++){
            for (k = 0; k < yLength; k++, cnt++, pb++){
                if (pb->bt != Empty && onDraw.find(cnt) == onDraw.end()){
                    pad = NULL;
                    for (d = 0; d < 6; d++)
                    if (k + dy[d] >= 0 && k + dy[d] < yLength){
                        if (i + dx[d] < 0){
                            if (AdjChunk[2])
                            pad = AdjChunk[2]->block + cnt + is * (xLength - 1);
                        }
                        else if (i + dx[d] >= xLength){
                            if (AdjChunk[3])
                            pad = AdjChunk[3]->block + cnt - is * (xLength - 1);
                        }
                        else if (j + dz[d] < 0){
                            if (AdjChunk[0])
                            pad = AdjChunk[0]->block + cnt + js * (zLength - 1);
                        }
                        else if (j + dz[d] >= zLength){
                            if (AdjChunk[1])
                            pad = AdjChunk[1]->block + cnt - js * (zLength - 1);
                        }
                        else {
                            pad = block + cnt + dx[d] * is + dz[d] * js + dy[d];
                        }
                        
                        if (pad && pad->bt == Empty){
                            break;
                        }
                    }
                    if (pad && pad->bt == Empty){
                        onDraw.insert(cnt);
                    }
                }
            }
        }
    }
    #ifdef boooom
    tttt++;
    std::ofstream out;
    out.open("er.txt", std::ios::app);
    out << "CheckAdEmpty " << tttt << std::endl;
    out.close();
    #endif
}

void Chunk::Display(){
    GLint modelLoc = glGetUniformLocation(ChunkManager::shader.Program, "model");
    glm::vec3 WorldPos = glm::vec3(Position.x * xLength, 0.0f, Position.y * zLength);
    glm::vec3 LocPos;
    for (std::set<int>::iterator it = onDraw.begin(); it != onDraw.end(); it++){
        LocPos = glm::vec3(*it / (yLength * zLength),
                            *it & yLength - 1,
                            *it / yLength & zLength - 1);
                            
        glm::mat4 model = glm::translate(glm::mat4(), WorldPos + LocPos);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Draw a cube
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    #ifdef Fuck
        std::cout << onDraw.size() << std::endl;
    #endif
}

void Chunk::Load(int x, int z){
    
    Position = glm::vec2(x, z);
    
    char a[10], b[10];
    itoa(x, a, 10);
    itoa(z, b, 10);
    std::string FileName = ChunkFilePath + a + "_" + b;
    FILE *fp = NULL;
    #ifdef boooom
        std::ofstream out;
        out.open("er.txt", std::ios::app);
        out << "Chunk Load" << std::endl;
    #endif
    if ((fp = fopen(FileName.data(), "r")) == NULL){
        #ifdef boooom
            out << "Write" << std::endl;
        #endif
        randomProduce();
        WrittenBack();
    }
    else {
        #ifdef boooom
            out << "Read" << std::endl;
        #endif
        fgets(temp, FileSize, fp);
        #ifdef boooom
            out << "Read File Over File size:" << std::string(temp).length() << std::endl;
        #endif
        char *p = temp;
        //Block *pb = block;
        for (int i = 0, j, temp; i < blockNum; i++){
            while (*p == ' ') p++;
            for (j = 0;*p >= '0' && *p <= '9'; p++) {
                j = j * 10 + *p - '0';
            }
            block[i].bt = BlockType(j);
        }
        #ifdef boooom
            out << "Read Int Over" << std::endl;
        #endif
    }
    //fclose(fp);
    for (int i = 0; i < 4; i++){
        AdjChunk[i] = NULL;
    }
    CheckAdEmpty();
    #ifdef boooom
        out << "Chunk Load Over" << std::endl;
        out.close();
    #endif
}

void Chunk::WrittenBack(){
    #ifdef Fuck
        std::cout << "WrittenBack" << Position.x << " " << Position.y << std::endl;
    #endif
    char a[10], b[10];
    itoa(Position.x, a, 10);
    itoa(Position.y, b, 10);
    std::string FileName = ChunkFilePath + a + "_" + b;
    std::ofstream out;
    out.open(FileName.data());
    std::string temp = "";
    for (int i = 0; i < blockNum; i++){
        temp += itoa((int(block[i].bt)), a, 10);
        if (i != blockNum - 1) temp += " ";
    }
    out << temp;
    out.close();
}

//End class Chunk
////////////////////////////////////////////////////////////////////////////////
//class Chunk Manager
ChunkManager::ChunkManager(int radius){
    #ifdef boooom
        std::ofstream out;
        out.open("er.txt", std::ios::app);
        out << "CM C" << std::endl;
    #endif
    r = radius;
    for (int i = 0; i < num; i++){
        loaded[i] = false;
        for (int j = 0; j < ceEventNum; j++){
            EventMap[i][j] = false;
        }
    }
    ChunkPosMap.clear();
    
    // Event System init
    EventTimeHead = EventTimeTail = 0;
    while (!EventQue.empty()) EventQue.pop();
}

ChunkManager::~ChunkManager(){
    for (int i = 0; i < num; i++){
        if (loaded[i]){
            chunk[i].WrittenBack();
        }
    }
}

Shader ChunkManager::shader;

GLfloat ChunkManager::vertices[10000];

GLuint ChunkManager::VBO = 0;
GLuint ChunkManager::containerVAO = 0;

void ChunkManager::Load(){
    #ifdef boooom
        std::ofstream out;
        out.open("er.txt");
        out << "Load begin" << std::endl;
    #endif
    shader = Shader("cube.vs", "cube.frag");
    // Bind verteces array
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);
    #ifdef boooom
        out << "test point 1" << std::endl;
    #endif

	//Liu Modified here
		std::vector<glm::vec3> positions, normals;
		std::vector<glm::vec2> textures;
		
		LoadOBJ("Model/cube.obj", positions, textures, normals);

		int size_of_vertices = 0;
		for (int i = 0; i < positions.size(); ++i) {
			
			vertices[size_of_vertices++] = positions[i].x;
			vertices[size_of_vertices++] = positions[i].y;
			vertices[size_of_vertices++] = positions[i].z;
			vertices[size_of_vertices++] = normals[i].x;
			vertices[size_of_vertices++] = normals[i].y;
			vertices[size_of_vertices++] = normals[i].z;
			vertices[size_of_vertices++] = textures[i].x;
			vertices[size_of_vertices++] = textures[i].y;
		}

		size_of_vertices *= sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size_of_vertices, vertices, GL_STATIC_DRAW);

    #ifdef boooom
        out << "test point 1" << std::endl;
    #endif
    glBindVertexArray(containerVAO);
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
    glUniform1i(glGetUniformLocation(shader.Program, "material.diffuse"),  0);
    glUniform1i(glGetUniformLocation(shader.Program, "material.specular"), 1);
    #ifdef boooom

        out << "Load end" << std::endl;
        out.close();
    #endif
}

void ChunkManager::CheckPos(glm::vec3 PlayerPos){
    // Time in Event System
    EventTimeHead++;
    if (EventTimeTail < EventTimeHead) EventTimeTail = EventTimeHead;
    
    // static int lastx = 0;
    // static int lasty = 0;
    static int firstCheck = 1;
    static int tot = 0;
    static std::PII disp[num];
    int x = PlayerPos.x / Chunk::xLength;
    if (PlayerPos.x < 0) x--;
    int y = PlayerPos.z / Chunk::zLength;
    if (PlayerPos.z < 0) y--;
    if (firstCheck){
        for (int i = -r; i <= r; i++){
            for (int j = -r; j <= r; j++){
                if (SquareSum(i, j)<= r * r){
                    disp[tot++] = std::make_pair(i, j);
                }
            }
        }
        for (int i = 0; i < tot; i++){
            for (int j = i + 1; j < tot; j++){
                if (Modu(disp[i]) > Modu(disp[j])){
                    std::swap(disp[i], disp[j]);
                }
            }
        }
        firstCheck = 0;
    }
    
    // main check
    //if (lastx != x || lasty != y){
        for (int i = 0; i < num; i++)
        if (loaded[i]){
            int dx = chunk[i].Position.x - x;
            int dy = chunk[i].Position.y - y;
            if (dx * dx + dy * dy > r * r){
                //chunk[i].WrittenBack();
                PushEvent(1, i, ceWrittenBack);
            }
        }
        for (int i, j, k = 0, p = 0; p < tot; p++){
            i = disp[p].first + x;
            j = disp[p].second + y;
            if (ChunkPosMap.find(std::make_pair(i, j)) == ChunkPosMap.end()){
                for (; k < num && loaded[k]; k++);
                if (k < num){
                    //chunk[k].Load(x + i, y + j);
                    PushEvent(2, k, ceLoad, i, j);
                    //EventLoadPos.push(std::make_pair(x + i, y + i),);
                }
            }
        }
             
        // set Adjacent Chunk
        const int dx[4] = {0, 0, -1, 1};
        const int dy[4] = {-1, 1, 0, 0};
        for (int i = 0, k, cx, cy, tox, toy; i < num; i++)
        if (loaded[i]){
            cx = chunk[i].Position.x;
            cy = chunk[i].Position.y;
            int change = 0;
            for (int d = 0; d < 4; d++){
                tox = cx + dx[d];
                toy = cy + dy[d];
                Chunk *temp = chunk[i].AdjChunk[d];
                if (ChunkPosMap.find(std::make_pair(tox, toy)) != ChunkPosMap.end()){
                    k = ChunkPosMap[std::make_pair(tox, toy)];
                    chunk[i].AdjChunk[d] = chunk + k;
                }
                else {
                    chunk[i].AdjChunk[d] = NULL;
                }
                if (temp != chunk[i].AdjChunk[d]){
                    change = 1;
                }
            }
            if (change){
                //chunk[i].CheckAdEmpty();
                PushEvent(2, i, ceCheckAdEmpty);
            }
        }
        
        #ifdef boooom
            std::ofstream out;
            out.open("er.txt", std::ios::app);
            out << "loaded" << std::endl;
            for (int i = 0; i < num; i++)if (loaded[i]){
                out << i << " " << chunk[i].Position.x << " " << chunk[i].Position.y << std::endl;
            }
        #endif
    
    RunEvent();
    
    #ifdef boooom
        out << "pppp" << std::endl;
        out.close();
    #endif
    
    // lastx = x;
    // lasty = y;
    // std::cout << num << std::endl;
    // for (int i = 0; i < num; i++)
    // if (loaded[i]){
        // std::cout << chunk[i].Position.x << " " << chunk[i].Position.y << std::endl;
    // }
    // std::cout << std::endl;
}

void ChunkManager::Display(){
    shader.Use();
    const Camera &camera = Model::getCamera();
        // Positions of the point lights
        glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
        };
        
        // diliver viewPos
        GLint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
        // Set material properties
        glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);
    
        // Directional light
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        //glUniform3f(glGetUniformLocation(shader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.ambient"), 1.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.diffuse"), 0.0f, 1.0f, 0.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "dirLight.specular"), 0.0f, 0.0f, 1.0f);
        // Point light 1
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].linear"), 0.09);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].quadratic"), 0.032);
        // Point light 2
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].linear"), 0.09);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].quadratic"), 0.032);
        // Point light 3
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[2].linear"), 0.09);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[2].quadratic"), 0.032);
        // Point light 4
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[3].linear"), 0.09);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[3].quadratic"), 0.032);
        // SpotLight
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.linear"), 0.09);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.quadratic"), 0.032);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

    
    
    // Create camera transformations
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)camera.WIDTH / (GLfloat)camera.HEIGHT, 0.1f, 100.0f);
    // Get the uniform locations
    GLint viewLoc  = glGetUniformLocation(shader.Program, "view");
    GLint projLoc  = glGetUniformLocation(shader.Program, "projection");
    // Pass the matrices to the shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
        // Bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // Bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
    
    glBindVertexArray(containerVAO);
    
    for (int i = 0; i < num; i++)
    if (loaded[i]){
        chunk[i].Display();
    }
    
    glBindVertexArray(0);
}

void ChunkManager::RunEvent(){
    /*enum ChunkEvent{
        ceWrittenBack,
        ceLoad,
        ceCheckAdEmpty
    };*/
    
    #ifdef boooom
        std::ofstream out;
        out.open("er.txt", std::ios::app);
        out << "run event" << std::endl;
    #endif
    std::queue<int> p;
    int a[5] = {0};//time id type
    for (;!EventQue.empty() && (p = EventQue.front()).front() <= EventTimeHead; EventQue.pop()){
        for (int i = 0; i < 5 && !p.empty(); i++){
            a[i] = p.front();
            p.pop();
        }
        #ifdef boooom
        out << "test point " << std::endl;
        #endif
        int id = a[1];
        int type = a[2];
        
        #ifdef boooom
        
        out <<"run " << a[0] << " " << chunk[a[1]].Position.x << " " << chunk[a[1]].Position.y << " " << int(a[2]) << std::endl;
        
        #endif
        if (type == ceWrittenBack){
            loaded[id] = false;
            ChunkPosMap.erase(std::make_pair(chunk[id].Position.x, chunk[id].Position.y));
            chunk[id].onDraw.clear();
            chunk[id].WrittenBack();
        }
        else if (type == ceCheckAdEmpty){
            chunk[id].CheckAdEmpty();
        }
        else if (type == ceLoad){
            loaded[id] = true;
            ChunkPosMap[std::make_pair(a[3], a[4])] = id;
            chunk[id].Load(a[3], a[4]);
        }
        EventMap[id][type] = false;
        #ifdef boooom
        
        out <<"run " << a[0] << " " << chunk[a[1]].Position.x << " " << chunk[a[1]].Position.y << " " << int(a[2]) << std::endl;
        
        #endif
    }
    #ifdef boooom
    out << "run event over" << std::endl;
    out.close();
    #endif
}
void ChunkManager::PushEvent(int time, int chunkId, ChunkEvent type, int x, int y){
    if (EventMap[chunkId][type]){
        return;
    }
    EventMap[chunkId][type] = true;
    time = EventTimeTail += time;
    std::queue<int> a;
    while (!a.empty()) a.pop();
    a.push(time);
    a.push(chunkId);
    a.push(int(type));
    if (type == ceLoad){
        a.push(x);
        a.push(y);
    }
    EventQue.push(a);
    #ifdef boooom
    std::ofstream out;
    out.open("er.txt", std::ios::app);
    out <<"push " << time << " " << chunkId << " " << int(type) << std::endl;
    out.close();
    #endif
}

// End class ChunkManager
#endif
