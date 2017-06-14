#ifndef __OBJ_CPP__
#define __OBJ_CPP__

//OBJ
#include "obj.h"

using namespace std;

void LoadOBJ(std::string filename, 
	std::vector<glm::vec3> &positions, 
	std::vector<glm::vec2> &textures, 
	std::vector<glm::vec3> &normals)
{

	std::vector<glm::vec3>v;
	std::vector<glm::vec2>vt;
	std::vector<glm::vec3>vn;


	std::ifstream in(filename);
	if (!in) {
		cerr << "Can't open the file " << filename << endl;
	}

	string line, str;

	int Case = 0;
	while (getline(in, line)) {
		istringstream str_in(line);
		str_in >> str;
		if (str == "v") {
			GLfloat x, y, z;
			str_in >> x >> y >> z;
			v.push_back(glm::vec3(x, y, z));
		}
		else if (str == "vt") {
			GLfloat x, y;
			str_in >> x >> y;
			vt.push_back(glm::vec2(x, y));
		}
		else if (str == "vn") {
			GLfloat x, y, z;
			str_in >> x >> y >> z;
			vn.push_back(glm::vec3(x, y, z));
		}
		else if (str == "f") {
			vector<int> v_indices;
			vector<int> vt_indices;
			vector<int> vn_indices;
			while (str_in >> str) {
				int v_index, vt_index, vn_index;
				sscanf(str.data(), "%d/%d/%d", &v_index, &vt_index, &vn_index);
				//cout << v_index << " " << vt_index << " " << vn_index << " " << endl;
				v_indices.push_back(v_index);
				vt_indices.push_back(vt_index);
				vn_indices.push_back(vn_index);
			}

			//v0 v1 v2 v3 => (v0, v1, v2) (v0, v2, v3)
			for (int i = 1; i < v_indices.size()-1; ++i) {
				positions.push_back(v[v_indices[0] - 1]);
				textures.push_back(vt[vt_indices[0] - 1]);
				normals.push_back(vn[vn_indices[0] - 1]);

				positions.push_back(v[v_indices[i] - 1]);
				textures.push_back(vt[vt_indices[i] - 1]);
				normals.push_back(vn[vn_indices[i] - 1]);

				positions.push_back(v[v_indices[i+1] - 1]);
				textures.push_back(vt[vt_indices[i+1] - 1]);
				normals.push_back(vn[vn_indices[i+1] - 1]);
			}

		}
	}
}

void LoadOBJ(const std::string filename, GLuint &VAO, GLuint &size){
    std::vector<glm::vec3> v[3];
    std::vector<GLfloat> resultData;


	std::ifstream in(filename);
	if (!in) {
		cerr << "Can't open the file " << filename << endl;
	}

	string line, str;

	int Case = 0;
	while (getline(in, line)) {
		istringstream str_in(line);
		str_in >> str;
		if (str == "v") {
			GLfloat x, y, z;
			str_in >> x >> y >> z;
			v[0].push_back(glm::vec3(x, y, z));
		}
		else if (str == "vt") {
			GLfloat x, y;
			str_in >> x >> y;
			v[2].push_back(glm::vec3(x, y, 0.0f));
		}
		else if (str == "vn") {
			GLfloat x, y, z;
			str_in >> x >> y >> z;
			v[1].push_back(glm::vec3(x, y, z));
		}
		else if (str == "f") {
			vector<int> v_indices[3];
			while (str_in >> str) {
				int v_index, vt_index, vn_index;
				sscanf(str.data(), "%d/%d/%d", &v_index, &vt_index, &vn_index);
				//cout << v_index << " " << vt_index << " " << vn_index << " " << endl;
				v_indices[0].push_back(v_index);
				v_indices[2].push_back(vt_index);
				v_indices[1].push_back(vn_index);
			}

			//v0 v1 v2 v3 => (v0, v1, v2) (v0, v2, v3)
            glm::vec3 *p;
			for (int i = 1; i < v_indices[0].size()-1; ++i) {
                int a[] = {0, i, i + 1};
                for (int j = 0; j < 3; j++){
                    for (int k = 0; k < 3; k++){
                        p = &(v[k][v_indices[k][a[j]] - 1]);
                        resultData.push_back(p->x);
                        resultData.push_back(p->y);
                        if (k != 2){
                            resultData.push_back(p->z);
                        }
                    }
                }
			}

		}
	}
    
    size = resultData.size();
    GLfloat *vertices = &resultData[0];
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    
}

#endif