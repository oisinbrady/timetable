#include <stdio.h>
#include "functions.h"
int main() {
    char* folderPath = getFolder();
    Module *modulesList = readModules(folderPath);
    Scheme *schemesList  = readSchemes(folderPath);
    moduleInfo(modulesList, schemesList);
    int **teachingTimes = readTimes(folderPath);
    buildTimetable(modulesList, schemesList, teachingTimes);
    return 0;
}













/*for (int i = 0; i < 95; i++){
printf("%s \n", schemesList[i].schemeCode);
printf("%d \n", schemesList[i].yearOfStudy);
printf("%d \n", schemesList[i].numberOfStudents);
for(int i = 0; i < schemesList[i].numberOfStudents; i++){
printf("%s \n", (char *) schemesList[i].coreModule);
}}*/

//print 2d array of times TEST
/*
for (int i = 0; i <  7; i++){
        printf("\n");
        for (int j = 0; j < 9; j++) {
            printf("%d ", teachingTimes[i][j]);
        }
    }
 */