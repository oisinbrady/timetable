#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "functions.h"

char *getFolder(char* argv[]) {
    int maxInputSize = 20;
    char *fileName = malloc((size_t) maxInputSize);
    printf("Enter the directory name of timetable files: ");
    scanf("%s", fileName);
    //char *realFileName = malloc((size_t) maxInputSize);
    //realFileName[0] = '/';
    //fileName = strcat(realFileName, fileName);
    return fileName;
    //
}

/**
 * Create an array of modules (structs)
 * @param file the file containing all module details
 * @return an array of struct Modules
 */
Module * readModules(char *file){
    //attempt to open the file.txt file in the user specified directory path
    char* modules = "/modules.txt";
    file = strcat(file,modules);
    FILE *fileDirectory = fopen(file, "r");
    //each module has all four variables:
    char moduleID[8];
    int semester;
    char lectureAmountAndHr[4];
    char pracAmountAndHr[4];
    //find number of file (lines) in the file
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
    //iterate over each line in file.txt to record each module
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
Scheme * readSchemes(char *file, Module *modulesList){
    size_t fileLength = strlen(file);
    file[fileLength-12] = '\0'; //remove "/modules.txt" from current file directory
    char *schemes = strcat(file,"/schemes.txt"); //TODO OS specific!
    FILE *fileDirectory = fopen(schemes, "r");
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
        char schemeCode[5];
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
        fgets(coreModules, sizeof(coreModules), fileDirectory);//retrieve the remaining characters on the line
        //instantiate a new newCoreModule struct
        char *moduleID = malloc(sizeof(char *) * 7);
        scheme->coreModule = NULL;
        if(numberOfCoreModules > 0){
            int currentModuleIndex = sizeof(coreModules) - 8;
            for(int i = 0; i < numberOfCoreModules; i ++){
                //create a new module struct
                CoreModule *newCoreModule = malloc(sizeof(*newCoreModule) + 1);
                //create a substring which is one of the core moduleID's and then add it to the newCoreModule->moduleID
                strcpy(moduleID, &coreModules[currentModuleIndex]);
                coreModules[currentModuleIndex - 1] = '\0';
                currentModuleIndex -= 8;
                //assign the module's ID
                strcpy(newCoreModule->moduleID, moduleID);
                //add semester value to each core module
                for(int j = 0; j < numberOfModules; j++){
                    if(strncmp(moduleID, modulesList[j].moduleID, 7) == 0){
                        newCoreModule->semester = modulesList[j].semester;
                    }
                }
                newCoreModule->nextCoreModule = scheme->coreModule;
                scheme->coreModule = newCoreModule; //the new core module is now the array
            }
        }
        //add the instantiated struct to an array of structs
        listOfSchemes[moduleIndex] = *scheme;
        moduleIndex++;
        //free the memory allocated to the new instance of Scheme, CoreModule & moduleID to prevent memory leaks
        free(moduleID);
        free(scheme);
    }
    free(fileDirectory);
    return listOfSchemes;
}

int ** readTimes(char *file) {
    size_t fileLength = strlen(file);
    file[fileLength-12] = '\0';
    //create string directory path for modules.txt
    char *teachingTimes = strcat(file, "/times.txt"); //TODO this will be OS specific
    //attempt to open the modules.txt file in the user specified directory path
    FILE *fileDirectory = fopen(teachingTimes, "r");
    //Teaching times have two variables: the day(Mon-Sun) & the teaching hours(9-6)
    int r = 7; //Monday - Sunday
    int c = 9; //9 available teaching slots per day
    //assign a dynamic 2d array
    int ** times = (int **)malloc(r * sizeof(int *));
    for (int i = 0; i < r; ++i) {
        times[i] = (int *)calloc(c * sizeof(int) ,sizeof(int));
    }
    int teachingSlotsUsed;
    int currentSession;
    int nextSession;
    for (int i = 0; i < r ; ++i) {
        fscanf(fileDirectory, "%*s %d", &teachingSlotsUsed);
        fscanf(fileDirectory, "%d", &currentSession);
        fscanf(fileDirectory, "%d", &nextSession);
        for (int j = 0; j < teachingSlotsUsed; ++j) {

            //if the teaching slot is 1 hour long
            if(nextSession - currentSession < 2){
                times[i][currentSession - 9] = 1; //element contents dictate the length of the session
            }
            else{
                times[i][currentSession - 9] = 2;
            }
            currentSession = nextSession;
            fscanf(fileDirectory, "%d", &nextSession);
        }
    }
    return times;
}