#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

/**
 * print the menu options on the CLI
 */
void printMenu(){
    printf("\nOptions: \n1 = query module\n2 = make timetable\n3 = quit\n");
}

/**
 * this menu will continuosly loop and carry out commands according to the users option(s) until they select 3
 * to quit the program
 */
int menuLoop(Module *modulesList, Scheme *schemesList, int **teachingTimes){
    printf("--------------------------------------------------------------------------");
    printMenu();
    int option;

    printf("Enter Option: \n");
    scanf(" %d", &option);
    switch(option){
        case 1:
            moduleInfo(modulesList, teachingTimes, schemesList);
            return 1;
        case 2:
            buildTimetable(modulesList, schemesList, teachingTimes);
            return 2;
        case 3:
            return 3;
        default: printf("Invalid input!");
        }
        printf("\n\n");
        printf("--------------------------------------------------------------------------\n");
}

/**
 * initial statements responsible for parsing the files in the user determined folder
 * after which the program goes into a menu loop where program flow is determined by the users input
 * @return int exit code
 */
int main() {
    char* folderPath = getFolder();
    Module *modulesList = readModules(folderPath);
    Scheme *schemesList  = readSchemes(folderPath, modulesList);
    int **teachingTimes = readTimes(folderPath);
    do{}while(menuLoop(modulesList, schemesList, teachingTimes) != 3);
    free(folderPath);
    free(modulesList);
    free(schemesList);
    //free calloc'd memory in the 2D times array
    for (int i = 0; i < 7; i++)
    {
        int* currentIntPtr = teachingTimes[i];
        free(currentIntPtr);
    }
    free(teachingTimes);
    return 0;
}











//TEST data sets with print blocks//

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
    printf("\n");
 */