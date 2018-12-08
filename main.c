#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void printMenu(){
    printf("\nOptions: \n1 = query module\n2 = make timetable\n3 = quit\n");
}

void menuLoop(Module *modulesList, Scheme *schemesList, int **teachingTimes){
    printf("-------------------------------------------------------------");
    printMenu();
    int option;
    while(true){
        scanf("%d", &option);
        switch(option){
            case 1:
                moduleInfo(modulesList, schemesList);
                break;
            case 2:
                buildTimetable(modulesList, schemesList, teachingTimes);
                break;
            case 3:
                exit(0); //TODO this does not work...
                //abort();
            default: printf("Invalid input!");
        }
        printf("--------------------------------------------------------------\n");
   }
}

int main(int argc, char*argv[]) {
    char* folderPath = getFolder(argv);
    Module *modulesList = readModules(folderPath);
    Scheme *schemesList  = readSchemes(folderPath, modulesList);
    int **teachingTimes = readTimes(folderPath);
    menuLoop(modulesList, schemesList, teachingTimes);
    return 0;
}






//PRINT TESTS//

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