
#ifndef _DEADLINE_CPP_
#define _DEADLINE_CPP_

#include "deadline.h"
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

Deadline::Deadline(){
     this->numitem=0; 
     q = new queue<Registration *>;  
     
     };
Deadline::~Deadline(){
    delete q;
}
void Deadline::add(Registration *A, int identifier) {
    // consider if it is empty?
    this->q->push(A);
    this->numitem++ ;}
#endif