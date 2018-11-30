#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "functions.h"

char *getFolder() {
    int maxInputSize = 100;
    char *fileDirectory = malloc((size_t) maxInputSize);
    printf("Enter the directory name of timetable files: ");
    scanf("%s", fileDirectory);
    return fileDirectory;
    //
}

/**
 * Create an array of modules (structs)
 * @param file the file containing all module details
 * @return an array of struct Modules
 */
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

/**
 * Create an array of schemes (structs)
 * @param file the file containing all scheme details
 * @return an array of schemes
 */
Scheme * readSchemes(char *file){
    size_t fileLength = strlen(file);
    file[fileLength-12] = '\0'; //remove "/modules.txt" from current file directory
    char *schemes = strcat(file,"\\schemes.txt"); //TODO OS specific!
    FILE *fileDirectory = fopen(schemes, "r");
    if(fileDirectory == NULL) {
        perror("Error opening file: 'schemes.txt' ");
        exit(-1);
    }
    //find the total number of schemes in the text file
    numberOfSchemes = 0;
    while(!feof(fileDirectory)){
        int ch =  fgetc(fileDirectory);
        if(ch == '\n'){
            numberOfSchemes++;
        }
    }
    rewind(fileDirectory);
    Scheme *listOfSchemes = calloc((size_t) numberOfSchemes, (sizeof(Scheme) + 1));
    //iterate over each line in schemes.txt to record each module
    int moduleIndex = 0;
    while(!feof(fileDirectory)){
        char schemeCode[4];
        int yearOfStudy;
        int numberOfStudents;
        int numberOfCoreModules;
        //allocate memory for a new instance of module
        Scheme *scheme = malloc(sizeof(*scheme) + 1); //+ 3 for the 3 terminating characters in each char sequence
        //assign all module information from .txt file to their subsequent struct (Module) values
        fscanf(fileDirectory,"%s", schemeCode);
        strcpy(scheme->schemeCode, schemeCode);
        fscanf(fileDirectory,"%d", &yearOfStudy);
        scheme->yearOfStudy = yearOfStudy;
        fscanf(fileDirectory,"%d", &numberOfStudents);
        scheme->numberOfStudents = numberOfStudents;
        fscanf(fileDirectory, "%d", &numberOfCoreModules);
        scheme->numberOfCoreModules = numberOfCoreModules;
        // the size of the string of core modules is dependent on the numberOfCoreModules variable
        char coreModules[(numberOfCoreModules*7)+11];
        fgets(coreModules, sizeof(coreModules), fileDirectory);
        //TODO create a struct for a core module node which holds its name and a pointer to the next struct
        //instantiate a new coreModule struct
        CoreModule *coreModule = malloc(sizeof(*coreModule) + 1);
        if(numberOfCoreModules > 0){
            for(int i = 0; i < numberOfCoreModules; i ++){
                //create a substring which is one of the core moduleID's and then add it to the coreModule->moduleID
                char moduleID[7];
                strcpy(moduleID, &coreModules[sizeof(coreModules)-8]);
                coreModules[sizeof(coreModules)-9] = '\0';
                //coreModules[length-8] = '\0'; //remove "/modules.txt" from current file directory
            }
        }

        //add the instantiated struct to an array of structs
        listOfSchemes[moduleIndex] = *scheme;
        moduleIndex++;
        //free the memory allocated to the new instance of Scheme and CoreModule to prevent memory leaks
        free(coreModule);
        free(scheme);
    }
    free(fileDirectory);
    return listOfSchemes;
}