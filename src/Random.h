#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdlib.h>
#include <time.h>

class Random{
public:
    static void Init(){
        srand(time(NULL));
    }
    static int GetInt(int v){
        int c = int(1.0 * rand() / RAND_MAX * v);
        return c - (c == v);
    }
    static int GetInt(int L, int R){
        int c = L + int(1.0 * rand() / RAND_MAX * (R - L));
        return c - (c == R);
    }   
};

#endif
