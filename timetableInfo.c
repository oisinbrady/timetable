#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include "functions.h"

void moduleInfo(Module * modulesList, Scheme * schemesList){
    char moduleID[7];
    printf("Enter a module code: ");
    scanf("%s", moduleID);
    for(int i = 0; i < numberOfModules; i++){
        if(strncmp(moduleID, modulesList[i].moduleID, 7) == 0){
            printf("Semester: %d\n",modulesList[i].semester);
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
                //print all modules in the scheme except the current module entered (these are clashes)
                for(int k = 0; k < coreModuleIndex; k++){
                    printf("%s \n", (char *) schemesList[k].coreModule);
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