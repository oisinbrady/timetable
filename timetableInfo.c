#include <stdio.h>
#include <memory.h>
#include "functions.h"

void moduleInfo(Module * modulesList, Scheme * schemesList){
    char moduleID[7];
    printf("Enter a module code: ");
    scanf("%s", moduleID);
    for(int i = 0; i < numberOfModules; i++){
        if(strncmp(moduleID, modulesList[i].moduleID, 7) == 0){
            printf("%d\n",modulesList[i].semester);
            break;
        }
    }
    bool found = false;
    CoreModule *currentModule = schemesList->coreModule;
    while(!found){
        for(int i = 0; i < schemesList->numberOfCoreModules; i++){
            if(strncmp(currentModule->moduleID, moduleID, 7) == 0){
                printf("%d\n",schemesList[i].numberOfStudents);
                while(currentModule->nextCoreModule != NULL){ //TODO clarify on client requirements here
                    printf("%s \n", (char *) currentModule->moduleID);
                    currentModule = currentModule->nextCoreModule;
                }
                printf("%s \n", (char *) currentModule->moduleID);
                found = true;
            }
        }
        currentModule = currentModule->nextCoreModule;
    }
}