#include "treat.h"

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <sstream>
TreatLoc::TreatLoc(){
    return;
}
TreatLoc::TreatLoc(float* address){
    this->addr[0] = address[0];
    this->addr[1] = address[1];
    this->capacity = 50;
    this->occupied = 0;
    /*for(int i = 0; i < capacity; i++){
        Registration* patient[50] = NULL;
    }*/
    
}            
        
            
            