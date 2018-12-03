#include <malloc.h>
#include <memory.h>
#include "functions.h"



void push(CoreModule *header, char *moduleID)
{
    CoreModule *newCoreModule = malloc(sizeof(*newCoreModule) + 1);
    strcpy(newCoreModule->moduleID, moduleID);
    newCoreModule->nextCoreModule = header;
    header = newCoreModule;
}