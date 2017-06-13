//OBJ
#include "obj.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
				sscanf_s(str.data(), "%d/%d/%d", &v_index, &vt_index, &vn_index);
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


void ExportOBJ()
{

}

