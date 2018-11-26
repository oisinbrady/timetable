#include <stdio.h>
#include "functions.h"
int main() {
    //create an array of struct = return struct array of readModules
    Module *modulesList = readModules();
    for(int i = 0; i < sizeof(modulesList); i++){
        printf(modulesList[i].moduleID);
        printf("\n");
    }
    return 0;
}