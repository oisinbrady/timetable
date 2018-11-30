#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <zconf.h>
#include "functions.h"

char *getFolder() {
    int maxInputSize = 100;
    char *fileDirectory = malloc((size_t) maxInputSize);
    printf("Enter the directory name of timetable files: ");
    scanf("%s", fileDirectory);
    return fileDirectory;
    //
}

Module * readModules(char *file){
    //create string directory path for modules.txt
    char *modules = strcat(file,"/modules.txt"); //TODO this will be OS specific
    //attempt to open the modules.txt file in the user specified directory path
    FILE *fileDirectory = fopen(modules, "r");
    if(file == NULL) {
        perror("Error opening file: 'modules.txt' ");
        exit(-1);
    }
    //each module has all four variables:
    char moduleID[8];
    int semester;
    char lectureAmountAndHr[4];
    char pracAmountAndHr[4];
    //find number of modules (lines) in the file
    numberOfModules = 0;
    while(!feof(fileDirectory)){
        int ch =  fgetc(fileDirectory);
        if(ch == '\n'){
            numberOfModules++;
        }
    }
    rewind(fileDirectory); //go back to the beginning of the file
    //create a dynamic array whose size is based of the numberOfModules currently added
    //N.B calloc() assigns each element value to zero initially
    Module *listOfModules = calloc((size_t) numberOfModules, (sizeof(Module) + 3));
    //iterate over each line in modules.txt to record each module
    int moduleIndex = 0;
    while(fscanf(fileDirectory, "%s %d %s %s\n", moduleID, &semester, lectureAmountAndHr, pracAmountAndHr)!= EOF){
        //allocate memory for a new instance of module
        Module *module = malloc(sizeof(*module) + 3); //+ 3 for the 3 terminating characters in each char sequence
        //assign all module information from .txt file to their subsequent struct (Module) values
        strcpy(module->moduleID, moduleID);
        module->semester = semester;
        strcpy(module->lectureAmountAndHr, lectureAmountAndHr);
        strcpy(module->pracAmountAndHr, pracAmountAndHr);
        //add the instantiated struct to an array of structs
        listOfModules[moduleIndex] = *module;
        moduleIndex++;
        //free the memory allocated to the new instance of Module to prevent memory leaks
        free(module);
    }
    free(fileDirectory);
    return listOfModules;
}

Scheme * readSchemes(char *file){
    char *schemes = strcat(file,"/schemes.txt");
    FILE *fileDirectory = fopen(schemes, "r");
    if(file == NULL) {
        perror("Error opening file: 'schemes.txt' ");
        exit(-1);
    }
    int x;
    char schemeCode[4];
    int yearOfStudy;
    int numberOfStudents;
    int numberOfCoreModules;
    char coreModules[x]; //TODO
    //find the total number of schemes in the text file
    numberOfSchemes = 0;
    while(!feof(fileDirectory)){
        int ch =  fgetc(fileDirectory);
        if(ch == '\n'){
            numberOfSchemes++;
        }
    }
    Scheme *listOfSchemes = calloc((size_t) numberOfSchemes, (sizeof(Scheme) + 3));
    //iterate over each line in modules.txt to record each module
    int moduleIndex = 0;
    while(fscanf(fileDirectory, "%s %d %d %d %c \n", schemeCode, &yearOfStudy, &numberOfStudents, &numberOfCoreModules,
                 coreModules)!= EOF){
        //allocate memory for a new instance of module
        Scheme *scheme = malloc(sizeof(*scheme) + 3); //+ 3 for the 3 terminating characters in each char sequence
        //assign all module information from .txt file to their subsequent struct (Module) values
        strcpy(scheme->schemeCode, schemeCode);
        scheme->yearOfStudy = yearOfStudy;
        scheme->numberOfStudents = numberOfStudents;
        scheme->numberOfCoreModules = numberOfCoreModules;
        //initialise an char array of size = numberOfCoreModules
        char *coreModules = malloc((size_t) maxInputSize);      //TODO
        scheme->coreModules = coreModules;


        //add the instantiated struct to an array of structs
        listOfSchemes[moduleIndex] = *scheme;
        moduleIndex++;
        //free the memory allocated to the new instance of Module to prevent memory leaks
        free(scheme);
    }
    free(fileDirectory);
    return listOfSchemes;
}