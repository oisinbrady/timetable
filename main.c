#include <stdio.h>
#include "functions.h"
int main() {
    char* folderPath = getFolder();
    Module *modulesList = readModules(folderPath);
    Scheme *schemesList  = readSchemes(folderPath);
    moduleInfo(modulesList, schemesList);
    int ** teachingTimes = readTimes(folderPath);
    return 0;
}

/*for (int i = 0; i < 95; i++){
printf("%s \n", schemesList[i].schemeCode);
printf("%d \n", schemesList[i].yearOfStudy);
printf("%d \n", schemesList[i].numberOfStudents);
for(int i = 0; i < schemesList[i].numberOfStudents; i++){
printf("%s \n", (char *) schemesList[i].coreModule);
}}*/