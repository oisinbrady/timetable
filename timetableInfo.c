#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "functions.h"

void initialiseClashArray(Scheme scheme, int semester, const char *moduleID) {
    CoreModule *current = scheme.coreModule; //pointer to the current core module
    //create an array which holds all found clashes

    int clashArrayIndex = 0;
    for (int i = 0; i < scheme.numberOfCoreModules; ++i) {
        if (strncmp(moduleID, current->moduleID, 7) != 0){
            if(current->semester == semester){ //add to clash array if core module is in the same semester as the entered module
                strcpy(clashArray[clashArrayIndex], current->moduleID);
                clashArrayIndex++;
            }
        }
        current = current->nextCoreModule;
    }
}

void addClash(Scheme scheme, int semester, const char *moduleID){
    CoreModule *current;
    current = scheme.coreModule;//current points to start of core modules list
    bool clashAlreadyRecorded = false;
    //iterate to the end of the linked list
    for(int i = 0; i < scheme.numberOfCoreModules; i++) {
        for (int j = 0; j < scheme.numberOfCoreModules; ++j) {
            //check if the current module is already recorded as a clash
            for (int k = 0; k < sizeof(clashArray)/ sizeof(clashArray[0]); ++k) {
                if (strncmp(current->moduleID, clashArray[k], 7) == 0) {
                    clashAlreadyRecorded = true;
                    break;
                }
            }
            //add the clashing module to the next available space in the clashes array
            if (!clashAlreadyRecorded && current->semester == semester && (strncmp(current->moduleID, moduleID , 7) != 0) ) {
                for (int l = 0; l < sizeof(clashArray); ++l) { //find the next free slot in the clash array
                    if (strncmp("\000", clashArray[l], 7) == 0) {
                        strcpy((char *) clashArray[l], current->moduleID);
                        break;
                    }
                }
            }else{
                clashAlreadyRecorded = false;
            }
        }
        current = current->nextCoreModule;
    }
}

void moduleInfo(Module * modulesList, int ** teachingTimes, Scheme * schemesList){
    char moduleID[7];
    int semester = 0;
    int numberOfStudents = 0;
    printf("Enter a module code: ");
    scanf("%s", moduleID);
    bool moduleExists = false;
    for (int i = 0; i < numberOfModules; ++i) {
        if(strncmp(moduleID, modulesList[i].moduleID, 7) == 0){
            moduleExists = true;
            break;
        }
    }
    if(moduleExists){
        for(int i = 0; i < numberOfModules; i++){
            if(strncmp(moduleID, modulesList[i].moduleID, 7) == 0){
                printf("Semester: %d\n",modulesList[i].semester);
                printf("Number of lectures: %.1s \n", modulesList[i].lectureAmountAndHr);
                printf("Length of lectures: %s \n", modulesList[i].lectureAmountAndHr + strlen(modulesList[i].lectureAmountAndHr) - 1);
                printf("Number of practicals: %.1s \n", modulesList[i].pracAmountAndHr);
                printf("Length of practicals: %s \n", modulesList[i].pracAmountAndHr + strlen(modulesList[i].pracAmountAndHr) - 1);
                semester = modulesList[i].semester;
                break;
            }
        }

        //find the number of students that are sitting the entered module

        bool clashArrayInit = false;
        for(int i = 0; i <= numberOfSchemes; i++) {
            CoreModule *currentCoreModule = schemesList[i].coreModule;
            CoreModule *clashes = malloc(sizeof(clashes));
            for (int j = 0; j < schemesList[i].numberOfCoreModules ; ++j) {
                if(strncmp(moduleID, currentCoreModule->moduleID,7) == 0) { //find any schemes with the entered moduleID
                    numberOfStudents += schemesList[i].numberOfStudents;
                    //TODO add all core modules in the same semester to data type holding all clashing modules
                    Scheme currentScheme = schemesList[i];
                    if(!clashArrayInit){
                        initialiseClashArray(currentScheme, semester, moduleID);
                        clashArrayInit = true;
                        currentCoreModule = currentCoreModule->nextCoreModule;
                        continue;
                    }
                    addClash(currentScheme, semester, moduleID);
                }
                currentCoreModule = currentCoreModule->nextCoreModule;
            }
        }
        printf("Number of students: %d \n", numberOfStudents);
        printf("Clashing modules: ");
        for(int i = 0; i< sizeof(clashArray)/ sizeof(clashArray[0]); i++){
            printf("%s ", clashArray[i]);
        }
        printf("\n");
    }
    else{
        printf("This module does not exist (this is a case sensitive search)\n");
    }
    printf("\nAvailable slots for teaching on a particular day are indicated by 1\n");
    //print out available teaching times
    printf("\n\t9:00\t10:00\t11:00\t12:00\t13:00\t14:00\t15:00\t16:00\t17:00");
    for (int i = 0; i <  7; i++){
        printf("\n");
        switch(i){
            case 0:
                printf("Mon");
                break;
            case 1:
                printf("Tue");
                break;
            case 2:
                printf("Wed");
                break;
            case 3:
                printf("Thu");
                break;
            case 4:
                printf("Fri");
                break;
            case 5:
                printf("Sat");
                break;
            case 6:
                printf("Sun");
                break;
        }
        for (int j = 0; j < 9; j++) {
            printf("\t  %d\t", teachingTimes[i][j]);
        }
    }
    printf("\n");
}

