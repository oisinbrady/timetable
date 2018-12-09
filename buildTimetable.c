#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include "functions.h"

bool isClash(struct timetableCell cell, Module currentRelevantModule, Scheme* relevantSchemesList, int numberOfRelevantSchemes){
    CoreModule *currentCellModule = cell.nextCoreModule;
    bool clashArrayInit = false;
    int numberOfStudents = 0; //if this remains zero then return true (as a module with no students is not to be timetabled
    while(currentCellModule != NULL){ //this expression will be met once all cells have been added a core module once
        if(strncmp(currentRelevantModule.moduleID, currentCellModule->moduleID, 7) == 0){ //check if the lecture is already added to this day and time
            return true;
        }
        //TODO find the the scheme of currentCoreModule is the same scheme as relevantModule
        numberOfStudents = updateClashArray(relevantSchemesList, currentCellModule->moduleID, currentCellModule->semester, numberOfStudents, clashArrayInit);
        for (int i = 0; i < sizeof(clashArray)/ sizeof(clashArray[0]); ++i) {
            if(strncmp(currentRelevantModule.moduleID,clashArray[i], 7) == 0){
                return true;
            }
        }
        //else{
        //    currentCellModule = currentCellModule->nextCoreModule;
        //}
    }
    return false;
}

void buildTimetable(Module *modulesList, Scheme * schemesList, int ** teachingTimeSlots){
    int semester;
    int yearOfStudy;
    printf("\nEnter the semester for the timetable (1/2): ");
    scanf("%d", &semester);
    printf("\nEnter year of study: ");
    scanf("%d", &yearOfStudy);
    //TODO Build an academic timetable for one week of that semester.
    //TODO You should not timetable modules if there are no students taking them.
    //construct a 2D array of TimeTableCell structs
    int r = 7; //Monday - Sunday
    int c = 9; //9 available teaching slots per day
    TimeTableCell ** timeTable = malloc(r * sizeof(TimeTableCell));
    for (int i = 0; i < r; ++i) {
        timeTable[i] = calloc(c * sizeof(TimeTableCell) ,sizeof(TimeTableCell));
    }
    //TODO set the cells "boolean" value according to teachingTimesSlots array
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
    Scheme *relevantSchemes = calloc(numberOfRelevantSchemes, sizeof(Scheme));
    int relevantSchemeIndex = 0;
    for (int k = 0; k < numberOfSchemes; ++k) {
        if(schemesList[k].yearOfStudy == yearOfStudy){ //TODO do I need to copy all of these variables?
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

    //TODO find teaching hours for a module.. add up all relevant module teaching hours to get number of teaching hours in timetable

    int tmp = availableTeachingHours; //<----use this var
    int currentlyTaughtHours = 0;
    //TODO WHILE(timetable is not full)
    //TODO iterate through timetable cells and for each cell with bool value == 1: add next module from relevantModulesArray
    //TODO IF: current timeTableCell's linked list does not contain a module which belongs to the same scheme as next module
    //TODO decrement relevantModulesArray[current] lecture/practical amount accordingly (add all practicals first for a module then lectures)
    //TODO increment index of modulesArray (if currentModulesIndex > relevantModulesArray.length() THEN currentModulesIndex = 0)
    //TODO goto next timetable cell (if currentCellIndex > timetable.length() THEN currentCell = 0)
    //TODO increment the currentHours var by the length of the lec/prac just added (e.g. currentHours += &module.pracAmountAndTime[2])
    //build the timetable array
    int currentRelevantModule = 0; //index for the modules array
    while(currentlyTaughtHours <= totalHours){
        for (int i = 0; i < r; ++i) { //iterate through timetable cells
            for (int j = 0; j < c; ++j) {
                CoreModule *currentCoreModule = timeTable[i][j].nextCoreModule;
                CoreModule *coreModule = malloc(sizeof(coreModule)); //TODO remember to free() this!
                if(currentCoreModule == NULL){ //if the cell is empty
                    strcpy(coreModule->moduleID, relevantModules[currentRelevantModule].moduleID);
                    coreModule->semester = relevantModules[currentRelevantModule].semester;
                    coreModule->nextCoreModule = NULL;
                    currentCoreModule = coreModule;
                }
                //if there is no clash then add the currentCoreModule to the cell's list of coreModules
                if(!isClash(timeTable[i][j], relevantModules[currentRelevantModule], relevantSchemes ,numberOfRelevantSchemes)) {
                    currentCoreModule->nextCoreModule = timeTable[i][j].nextCoreModule;
                    timeTable[i][j].nextCoreModule = currentCoreModule;
                    int moduleLectureHours = ((atoi(&relevantModules[0].lectureAmountAndHr[2]))* (atoi(&relevantModules[0].lectureAmountAndHr[0])));
                    int modulePracticalHours = ((atoi(&relevantModules[0].pracAmountAndHr[2])) * (atoi(&relevantModules[0].pracAmountAndHr[0])));
                    currentlyTaughtHours += (moduleLectureHours + modulePracticalHours);
                }
                free(coreModule);
                currentRelevantModule++;
            }
        }
    }
    //TODO print out each cell's linked list
    printf("\n\t9:00\t10:00\t11:00\t12:00\t13:00\t14:00\t15:00\t16:00\t17:00");
    for (int i = 0; i <  7; i++){
        printf("\n");
        switch(i){
            case 0:
                printf("Mon");
                break;
            case 1:
                printf("Tue");
                break;
            case 2:
                printf("Wed");
                break;
            case 3:
                printf("Thu");
                break;
            case 4:
                printf("Fri");
                break;
            case 5:
                printf("Sat");
                break;
            case 6:
                printf("Sun");
                break;
        }
        for (int j = 0; j < 9; j++) {
            CoreModule *coreModuleToPrint = timeTable[i][j].nextCoreModule;
            while(coreModuleToPrint->nextCoreModule != NULL){
                printf("\t  %s\t", coreModuleToPrint->moduleID);
                coreModuleToPrint = coreModuleToPrint->nextCoreModule;
            }

        }
    }
    printf("\n");
}



