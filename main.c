#include <stdio.h>
#include "functions.h"
int main() {
    //create an array of struct = return struct array of readModules
    Module *modulesList = readModules();
    int size = sizeof(modulesList);
    printf("Number of modules: %d\n", size);
    for(int i = 0; i < sizeof(modulesList); i++){
        printf("%s\n", modulesList[i].moduleID);
        printf("%d\n", modulesList[i].semester);
        printf("%s\n", modulesList[i].lectureAmountAndHr);
        printf("%s\n", modulesList[i].pracAmountAndHr);
        printf("\n");
    }
    return 0;
}