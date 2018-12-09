#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include "functions.h"

bool isClash(struct timetableCell cell, Module currentRelevantModule, Scheme* relevantSchemesList){
    CoreModule *currentCellModule = cell.nextCoreModule;
    bool clashArrayInit = false;
    int numberOfStudents = 0; //if this remains zero then return true (as a module with no students is not to be timetabled
    while(currentCellModule != NULL){ //this expression will be met once all cells have been added a core module once
        if(strncmp(currentRelevantModule.moduleID, currentCellModule->moduleID, 7) == 0){ //check if the lecture is already added to this day and time
            return true;
        }
        numberOfStudents = updateClashArray(relevantSchemesList, currentCellModule->moduleID, currentCellModule->semester, numberOfStudents, clashArrayInit);
        for (int i = 0; i < sizeof(clashArray)/ sizeof(clashArray[0]); ++i) {
            if(strncmp(currentRelevantModule.moduleID,clashArray[i], 7) == 0){
                return true;
            }
        }
        currentCellModule = currentCellModule->nextCoreModule;
    }
    memset(clashArray, 0, sizeof(clashArray[0][0]) * 50 * 8); //reset the clash array
    return false;
}

void buildTimetable(Module *modulesList, Scheme * schemesList, int ** teachingTimeSlots){
    int semester;
    int yearOfStudy;
    printf("\nEnter the semester for the timetable (1/2): ");
    scanf("%d", &semester);
    printf("\nEnter year of study: ");
    scanf("%d", &yearOfStudy);
    //construct a 2D array of TimeTableCell structs
    int r = 7; //Monday - Sunday
    int c = 9; //9 available teaching slots per day
    TimeTableCell ** timeTable = malloc(r * sizeof(TimeTableCell));
    for (int i = 0; i < r; ++i) {
        timeTable[i] = calloc(c * sizeof(TimeTableCell) ,sizeof(TimeTableCell));
    }


    for (int j = 0; j < c; ++j) {
        for (int i = 0; i < r; ++i) {
            if(teachingTimeSlots[i][j] == 1){
                timeTable[i][j].available = 1;
            }
        }
    }

    //iterate over relevantModules and place any module that is in the same semester into a new array of modules
    int numberOfRelevantModules = 0;
    for (int k = 0; k < numberOfModules; ++k) { //find the number of modules in the same semester as int semester var
        if(modulesList[k].semester == semester){
            numberOfRelevantModules++;
        }
    }
    //iterate over relevantModules and place any module that is in the same semester into a new array of modules
    Module relevantModules[numberOfRelevantModules];
    int relevantModuleIndex = 0;
    for (int k = 0; k < numberOfModules; ++k) {
        if(modulesList[k].semester == semester){
            strcpy(relevantModules[relevantModuleIndex].pracAmountAndHr, modulesList[k].pracAmountAndHr);
            strcpy(relevantModules[relevantModuleIndex].lectureAmountAndHr, modulesList[k].lectureAmountAndHr);
            strcpy(relevantModules[relevantModuleIndex].moduleID, modulesList[k].moduleID);
            relevantModules[relevantModuleIndex].semester = modulesList[k].semester;
            relevantModuleIndex++;
        }
    }

    int numberOfRelevantSchemes = 0;
    for (int k = 0; k < numberOfModules; ++k) { //find the number of modules in the same semester as int semester var
        if(schemesList[k].yearOfStudy == yearOfStudy){
            numberOfRelevantSchemes++;
        }
    }
    Scheme *relevantSchemes = calloc((size_t)numberOfRelevantSchemes, sizeof(Scheme));
    int relevantSchemeIndex = 0;
    for (int k = 0; k < numberOfSchemes; ++k) {
        if(schemesList[k].yearOfStudy == yearOfStudy){
            strcpy(relevantSchemes[relevantSchemeIndex].schemeCode, schemesList[k].schemeCode);
            relevantSchemes[relevantSchemeIndex].yearOfStudy = schemesList[k].yearOfStudy;
            relevantSchemes[relevantSchemeIndex].coreModule = schemesList[k].coreModule;
            relevantSchemes[relevantSchemeIndex].numberOfCoreModules = schemesList[k].numberOfCoreModules;
            relevantSchemes[relevantSchemeIndex].numberOfStudents = schemesList[k].numberOfStudents;
            relevantSchemeIndex++;
        }
        if(relevantSchemeIndex > numberOfRelevantSchemes){
            break;
        }
    }

    int totalHours = 0;
    for (int l = 0; l < numberOfModules; ++l) {
        int moduleLectureHours = ((atoi(&relevantModules[0].lectureAmountAndHr[2]))* (atoi(&relevantModules[0].lectureAmountAndHr[0])));
        int modulePracticalHours = ((atoi(&relevantModules[0].pracAmountAndHr[2])) * (atoi(&relevantModules[0].pracAmountAndHr[0])));
        totalHours += (moduleLectureHours + modulePracticalHours);
    }

    int currentlyTaughtHours = 0;
    //build the timetable array
    int currentRelevantModule = 0; //index for the modules array
    int loopCounter = 0; //I need the var in the event that timetabling all modules is impossible with the given files and my implementation
    while(currentlyTaughtHours <= totalHours && loopCounter < 1000){
        for (int i = 0; i < 7; ++i) { //iterate through timetable cells
            for (int j = 0; j < 9; ++j) {

                if(currentRelevantModule > (sizeof(relevantModules)/ sizeof(relevantModules[0])-1)){
                    currentRelevantModule = 0;
                }

                int numberOfLectures = (relevantModules[currentRelevantModule].lectureAmountAndHr[0] - '0');
                int lengthOfLectures = relevantModules[currentRelevantModule].lectureAmountAndHr[2] - '0';
                int numberOfPracticals = relevantModules[currentRelevantModule].pracAmountAndHr[0] - '0';
                int lengthOfPracticals = relevantModules[currentRelevantModule].pracAmountAndHr[2] - '0';


                CoreModule *currentCoreModule = timeTable[i][j].nextCoreModule;
                CoreModule *coreModule = malloc(sizeof(coreModule));
                if(currentCoreModule == NULL && timeTable[i][j].available == true){ //if the cell is empty and available for teaching
                    strcpy(coreModule->moduleID, relevantModules[currentRelevantModule].moduleID);
                    coreModule->semester = relevantModules[currentRelevantModule].semester;
                    coreModule->nextCoreModule = NULL;
                    currentCoreModule = coreModule;
                    currentCoreModule->nextCoreModule = timeTable[i][j].nextCoreModule;
                    if(numberOfPracticals > 0){ //if the module has practicals not timetabled
                        numberOfPracticals--;
                        char pracItoC = (char) (numberOfPracticals + '0');
                        relevantModules[currentRelevantModule].pracAmountAndHr[0] = pracItoC;
                        coreModule->moduleID[6] = 'P';
                        //coreModule->moduleID[8] = '\000';
                        if(j+lengthOfPracticals <= 9){
                            timeTable[i][j].nextCoreModule = coreModule;
                        }
                        for (int k = 0; k < lengthOfPracticals; ++k) {
                            int jIndex = k + 1;
                            timeTable[i][jIndex].nextCoreModule = coreModule;
                        }
                        currentlyTaughtHours += (atoi(&relevantModules[currentRelevantModule].pracAmountAndHr[2]));
                    }
                    else if(numberOfLectures > 0){
                        numberOfLectures--;
                        char lectItoC = (char) (numberOfLectures + '0');
                        relevantModules[currentRelevantModule].lectureAmountAndHr[0] = lectItoC;
                        coreModule->moduleID[6] = 'L';
                        if(j+lengthOfLectures <= 9){
                            timeTable[i][j].nextCoreModule = coreModule;
                        }
                        for (int k = 0; k < lengthOfLectures; ++k) {
                            int jIndex = k + 1;
                            timeTable[i][jIndex].nextCoreModule = coreModule;
                        }
                        currentlyTaughtHours += (atoi(&relevantModules[currentRelevantModule].lectureAmountAndHr[2]));
                    }
                    currentRelevantModule++;
                }
                //if there is no clash then add the currentCoreModule to the cell's list of coreModules
                else if(!isClash(timeTable[i][j], relevantModules[currentRelevantModule], relevantSchemes) && timeTable[i][j].available == true && currentCoreModule != NULL) {
                    strcpy(coreModule->moduleID, relevantModules[currentRelevantModule].moduleID);
                    coreModule->semester = relevantModules[currentRelevantModule].semester;
                    coreModule->nextCoreModule = timeTable[i][j].nextCoreModule;
                    //prioritize timetabling practicals
                    if(numberOfPracticals > 0){ //if the module has practicals not timetabled
                        numberOfPracticals--;
                        char pracItoC = (char) (numberOfPracticals + '0');
                        relevantModules[currentRelevantModule].pracAmountAndHr[0] = pracItoC;
                        coreModule->moduleID[6] = 'P';
                        if(j+lengthOfPracticals <= 9){
                            timeTable[i][j].nextCoreModule = coreModule;
                        }
                        for (int k = 0; k < lengthOfPracticals; ++k) {
                            int jIndex = k + 1;
                            timeTable[i][jIndex].nextCoreModule = coreModule;
                        }
                        currentlyTaughtHours += (atoi(&relevantModules[currentRelevantModule].pracAmountAndHr[2]));
                    }
                    else if(numberOfLectures > 0){
                        numberOfLectures--;
                        char lectItoC = (char) (numberOfLectures + '0');
                        relevantModules[currentRelevantModule].lectureAmountAndHr[0] = lectItoC;
                        coreModule->moduleID[6] = 'L';
                        if(j+lengthOfLectures <= 9){
                            timeTable[i][j].nextCoreModule = coreModule;
                        }
                        for (int k = 0; k < lengthOfLectures; ++k) {
                            int jIndex = k + 1;
                            timeTable[i][jIndex].nextCoreModule = coreModule;
                        }
                        currentlyTaughtHours += (atoi(&relevantModules[currentRelevantModule].lectureAmountAndHr[2]));
                    }
                }
                currentRelevantModule++;
            }
        }
        loopCounter++;
    }

    printf("\t\t9:00\t\t\t\t     10:00\t\t\t\t         11:00\t\t\t\t\t     12:00\t\t\t\t\t     13:00\t\t\t\t\t     14:00\t\t\t\t     15:00\t\t\t\t\t     16:00\t\t\t\t\t     17:00");
    for (int i = 0; i <  7; i++){
        printf("\n\n");
        switch(i){
            case 0:
                printf("\n\nMon\t   ");
                break;
            case 1:
                printf("\n\nTue\t   ");
                break;
            case 2:
                printf("\n\nWed\t   ");
                break;
            case 3:
                printf("\n\nThu\t   ");
                break;
            case 4:
                printf("\n\nFri\t   ");
                break;
            case 5:
                printf("\n\nSat\t   ");
                break;
            case 6:
                printf("\n\nSun\t   ");
                break;
            default:break;
        }
        for (int j = 0; j < 9; j++) {
            CoreModule *coreModuleToPrint = timeTable[i][j].nextCoreModule;
            while(coreModuleToPrint != NULL){
             printf(" %s", coreModuleToPrint->moduleID);
             coreModuleToPrint = coreModuleToPrint->nextCoreModule;
             //printf("\n"); //TODO find out why this does not work as intended
            }
            printf("\t");
        }
    }
    printf("\n");


}



