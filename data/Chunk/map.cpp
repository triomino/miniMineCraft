#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdlib>

using namespace std;

enum BlockType{
    Empty,
    Grass,
    Dirt,
    Cloud,
    BlockTypeNum
};

void produceFileName(char *name, int i, int j)
{
	char tmp[200];
	itoa(i, name, 10);
	strcat(name, "_");
	itoa(j, tmp, 10);
	strcat(name, tmp);
}

int chunk[100][100][100][100];

const int size = 5;

int main(int argc, char const *argv[])
{
	int xlength=16, ylength=128, zlength=16;

	srand(time(NULL));

	char name[200];
	for (int i = -size; i <= size; i++)
		for (int j = -size; j <= size; j++) {
			for (int x = 0; x < xlength; ++x) {
				for (int z = 0; z < zlength; ++z) {
					chunk[i+size][j+size][x][z] = 32 + rand() % 8 - 4;
				}
			}

			int centerx = rand() % xlength;
			int centerz = rand() % zlength;
			int r = rand() % xlength;

			// smooth inside a chunk
			for (int x = 0; x < xlength; ++x) {
				for (int z = 0; z < zlength; ++z) {
					if ((x-centerx)*(x-centerx) + (z-centerz)*(z-centerz) <= r*r)
						chunk[i+size][j+size][x][z] = chunk[i+size][j+size][centerx][centerz];
				}
			}

		}

	// five Planes
	for (int k = 0; k < 5; k++) {
		int ii = rand() % (2*size+1);
		int jj = rand() % (2*size+1);

		double avg = 0;
		for (int x = 0; x < xlength; ++x)
			for (int z = 0; z < zlength; ++z) {
				avg += chunk[ii][jj][x][z];
			}
		avg /= (xlength*zlength);

		for (int x = 0; x < xlength; ++x)
			for (int z = 0; z < zlength; ++z) {
				chunk[ii][jj][x][z] = avg;
				if (ii > 0)
				chunk[ii-1][jj][x][z] = avg;
				if (jj > 0)
				chunk[ii][jj-1][x][z] = avg;
				if (ii > 0 && jj > 0)
				chunk[ii-1][jj-1][x][z] = avg;
				if (ii < size-1)
				chunk[ii+1][jj][x][z] = avg;
				if (jj < size-1)
				chunk[ii][jj+1][x][z] = avg;
				if (ii < size-1 && jj < size-1)
				chunk[ii+1][jj+1][x][z] = avg;
				if (ii < size-1 && jj > 0)
				chunk[ii+1][jj-1][x][z] = avg;
				if (ii > 0 && jj < size-1)
				chunk[ii-1][jj+1][x][z] = avg;
			}

	}

	for (int i = -size; i <= size; i++)
		for (int j = -size; j <= size; j++) {
			produceFileName(name, i, j);
			ofstream out(name);
			for (int x = 0; x < xlength; ++x) {
				for (int z = 0; z < zlength; ++z) {
					for (int l = 0; l < chunk[i+size][j+size][x][z]-1; ++l)
						out << Dirt << " ";
					out << Grass << " ";
					for (int l = chunk[i+size][j+size][x][z]; l < ylength; ++l)
					out << Empty << " ";
				}
			}
		}
	return 0;
}
