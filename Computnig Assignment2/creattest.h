#ifndef _CREATTEST_H_
#define _CREATTEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <random>
#include <fstream>
#include <sstream>
#include <time.h>
#include <string>
#define YEAR 2022
#define BOUNDARY 1000.0
using namespace std;

string randstr(int n);
int randnum(int min, int max);
float randfloat(float max, long unsigned int seed);
int creattest(int months, int num);

#endif
