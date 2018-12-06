#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include "functions.h"

void moduleInfo(Module * modulesList, Scheme * schemesList){
    char moduleID[7];
    int semester = 0;
    printf("Enter a module code: ");
    scanf("%s", moduleID);
    for(int i = 0; i < numberOfModules; i++){
        if(strncmp(moduleID, modulesList[i].moduleID, 7) == 0){
            printf("Semester: %d\n",modulesList[i].semester);
            semester = modulesList[i].semester;
            break;
        }
    }
    bool found = false;

    for(int i = 0; i <= numberOfSchemes; i++) {
        CoreModule *currentCoreModule = schemesList[i].coreModule;
        int coreModuleIndex = 0;
        for (int j = 0; j < schemesList[i].numberOfCoreModules; j++) {
            if (strncmp(currentCoreModule->moduleID, moduleID, 7) == 0) {
                printf("\nNumber of students: \n");
                printf("%d \n", schemesList[i].numberOfStudents);
                found = true;
                printf("\nClashing modules: \n");
                //print all modules clashes
                for(int k = 0; k < schemesList[i].numberOfCoreModules; k++){ //TODO if the core module is in the same scheme and same semester as the entered module then print it
                    for(int l = 0; l <numberOfModules; l++){
                        if(strcmp(modulesList[l].moduleID, currentCoreModule->moduleID) == 0){ //find the core Module in modules.txt
                            if(modulesList[l].semester == semester){ //print this module as a clash if the module is in the same semester as the user entered module
                                printf("%s \n", (char *) schemesList[k].coreModule);
                            }
                            break;
                        }
                    }

                }
                for(int l = coreModuleIndex + 1; l < schemesList[i].numberOfStudents; l++){
                    printf("%s \n", (char *) schemesList[l].coreModule);
                }
                break;
            }
            else if(currentCoreModule->nextCoreModule != NULL){
                currentCoreModule = currentCoreModule->nextCoreModule;
            }
            coreModuleIndex++;
        }
        if(found){
            break;
        }
    }
}