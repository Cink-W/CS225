#ifndef _DEADLINE_H_
#define _DEADLINE_H_
#include "registry.h"
#include "queue.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <cstdlib>



using namespace std;


class Deadline{
    public:
    Deadline();
    ~Deadline();
    void add(Registration *A, int identifier);
    int numitem;
    queue<Registration*>* q;

};
#endif