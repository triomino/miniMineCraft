#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdlib.h>
#include <time.h>
void RandomInit(){
    srand(time(NULL));
}
int RandomGetInt(int v){
    int c = int(1.0 * rand() / RAND_MAX * v);
    return c - (c == v);
}

#endif
