#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include "functions.h"

void moduleInfo(Module * modulesList, Scheme * schemesList){
    char moduleID[7];
    int semester = 0;
    int numberOfStudents = 0;
    printf("Enter a module code: ");
    scanf("%s", moduleID);

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

    CoreModule *potentialClashes = malloc(sizeof(potentialClashes) + 1);
    potentialClashes->nextCoreModule =NULL;
    //find the number of students that are sitting the entered module
    //char clashingModules[85];
    for(int i = 0; i <= numberOfSchemes; i++) {
        if(potentialClashes->nextCoreModule != NULL){
            break;
        }
        CoreModule *currentCoreModule = schemesList[i].coreModule;
        for (int j = 0; j < schemesList[i].numberOfCoreModules ; ++j) {
            if(strncmp(moduleID, currentCoreModule->moduleID,7) == 0){
                if(currentCoreModule->semester == semester){ //TODO implement semester onto coreModule
                    numberOfStudents += schemesList[i].numberOfStudents;
                    potentialClashes->nextCoreModule = schemesList[i].coreModule; //append all modules of this scheme to the linked list
                    break;
                }

            }

            currentCoreModule = currentCoreModule->nextCoreModule;
        }
    }
    printf("Number of students: %d \n", numberOfStudents);
    printf("Clashing modules: \n");
    CoreModule *restartList = malloc(sizeof(restartList) + 1);
    restartList->nextCoreModule = potentialClashes->nextCoreModule;
    for(int i = 0; i < numberOfModules; i++){
        if(potentialClashes->nextCoreModule == NULL){
            potentialClashes = restartList; //go back to the head of the linked list
        }
        while(potentialClashes->nextCoreModule != NULL){
            if(potentialClashes->moduleID != moduleID) {
                if (strncmp(potentialClashes->moduleID, modulesList[i].moduleID, 7) == 0) {
                    if (modulesList[i].semester == semester) {
                        printf("%s\n", modulesList[i].moduleID);
                    }
                }
            }
            potentialClashes = potentialClashes->nextCoreModule; //go to the next core module
        }
    }
}