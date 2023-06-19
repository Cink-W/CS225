#ifndef _TREAT_H_
#define _TREAT_H_

#include "registry.h"
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>

class TreatLoc {
    public:
            TreatLoc();
            TreatLoc(float* address);
            ~TreatLoc();

            float addr[2];
            int capacity = 50;
            int occupied = 0;
            Registration* patient[50];
};
#endif