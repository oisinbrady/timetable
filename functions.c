#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "functions.h"

Module * readModules(void){
    int maxInputSize = 100;
    char *fileDirectory = malloc((size_t) maxInputSize);
    printf("Enter the directory name of timetable files: ");
    scanf("%s", fileDirectory);
    //create string directory path for modules.txt
    char *modules = "";
    if((modules = malloc(strlen(fileDirectory)+strlen(modules)+1)) != NULL){
        modules[0] = '\0';   // ensures the memory is an empty string
        modules = strcat(fileDirectory,"\\modules.txt"); //TODO this will be OS specific - but program tested on linux environment so change to '/modules.txt'
    } else {
        perror("malloc failed!\n");
        exit(-1);
    }
    //attempt to open the modules.txt file in the user specified directory path
    FILE *file = fopen(modules, "r");
    if(file == NULL) {
        perror("Error opening file");
        exit(-1);
    }
    //each module has all four of variables
    char moduleID[8];
    int semester;
    char lectureAmountAndHr[4];
    char pracAmountAndHr[4];
    //create a dynamic array whose size is based of the numberOfModules currently added
    int numberOfModules = 0;
    Module *listOfModules = (Module *)malloc(sizeof(Module));
    //iterate over each line in modules.txt to record each module
    while(fscanf(file, "%s %d %s %s\n", moduleID, &semester, lectureAmountAndHr, pracAmountAndHr)!= EOF){
        numberOfModules++;
        realloc(listOfModules, sizeof(Module) * numberOfModules);
        Module *module = malloc(sizeof(Module));
        strcpy(module->moduleID, moduleID);
        module->semester = semester;
        strcpy(module->lectureAmountAndHr, lectureAmountAndHr);
        strcpy(module->pracAmountAndHr, pracAmountAndHr);
        //add the instantiated struct to an array of structs
        listOfModules[numberOfModules - 1] = *module;

    }
    return listOfModules;
}