#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include "functions.h"

/**
 * Check if there is a clash when attempting to add a module to the timetable's cell
 * This function returns a boolean value accordingly
 */
bool isClash(struct timetableCell cell, Module currentRelevantModule, Scheme* SchemesList){
    CoreModule *currentCellModule = cell.nextCoreModule;
    bool clashArrayInit = false;
    int numberOfStudents = 0; //if this remains zero then return true (as a module with no students is not to be timetabled
    while(currentCellModule != NULL){ //this expression will be met once all cells have been added a core module once
        if(strncmp(currentRelevantModule.moduleID, currentCellModule->moduleID, 7) == 0){ //check if the lecture is already added to this day and time
            return true;
        }
        numberOfStudents = updateClashArray(SchemesList, currentCellModule->moduleID, currentCellModule->semester, numberOfStudents, clashArrayInit);
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

/**
 * Construct the timetable as a 2D array and then print it onto the CLI.
 * The timetable will only add another module to the same time slot if there are not clashes detected and if the module
 * to add is in the semester that the user requested. Practical sessions are given priority in this implementation
 * i.e. this function will attempt to add practical session if it is given the choice between adding a lecture or practical.
 * @param modulesList the array of modules parsed from the specified folder's "modules.txt" file
 * @param schemesList the array of schemes parsed from the specified folder's "schemes.txt" file
 * @param teachingTimeSlots the 2D array of available teaching slots specified from the folder's "times.txt" file
 */
void buildTimetable(Module *modulesList, Scheme * schemesList, int ** teachingTimeSlots){
    int semester;
    int yearOfStudy;
    printf("\nEnter the semester for the timetable (1/2): ");
    scanf("%d", &semester);
    //construct a 2D array of TimeTableCell structs
    int r = 7; //Monday - Sunday
    int c = 9; //9 available teaching slots per day
    TimeTableCell ** timeTable = malloc(r * sizeof(TimeTableCell));
    for (int i = 0; i < r; ++i) {
        timeTable[i] = calloc(c * sizeof(TimeTableCell) ,sizeof(TimeTableCell));
    }

    //determine if a cell in the timetable can have lectures or practicals in it
    for (int j = 0; j < c; ++j) {
        for (int i = 0; i < r; ++i) {
            if(teachingTimeSlots[i][j] == 1){
                timeTable[i][j].available = 1;
            }
        }
    }

    //iterate across the modulesList array and determine how many modules are relevant to the timetable (by semester value)
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
    //calculate the total number of teaching hours that are required to in one week for all relevant modules (that are to be timetabled)
    int totalHours = 0;
    for (int l = 0; l < numberOfModules; ++l) {
        int moduleLectureHours = ((atoi(&relevantModules[0].lectureAmountAndHr[2]))* (atoi(&relevantModules[0].lectureAmountAndHr[0])));
        int modulePracticalHours = ((atoi(&relevantModules[0].pracAmountAndHr[2])) * (atoi(&relevantModules[0].pracAmountAndHr[0])));
        totalHours += (moduleLectureHours + modulePracticalHours);
    }

    //BUILD THE ARRAY//
    int currentlyTaughtHours = 0;
    int currentRelevantModule = 0; //index for the modules array
    int loopCounter = 0; //I need this var in the event that timetabling all modules is impossible with the given files and my implementation
    while(currentlyTaughtHours <= totalHours && loopCounter < 1000){
        for (int i = 0; i < 7; ++i) { //iterate through timetable cells
            for (int j = 0; j < 9; ++j) {

                //reset the index for the relevant module array
                if(currentRelevantModule > (sizeof(relevantModules)/ sizeof(relevantModules[0])-1)){
                    currentRelevantModule = 0;
                }

                //convert the ascii values for module lecture and practical information into integers
                int numberOfLectures = relevantModules[currentRelevantModule].lectureAmountAndHr[0] - '0';
                int lengthOfLectures = relevantModules[currentRelevantModule].lectureAmountAndHr[2] - '0';
                int numberOfPracticals = relevantModules[currentRelevantModule].pracAmountAndHr[0] - '0';
                int lengthOfPracticals = relevantModules[currentRelevantModule].pracAmountAndHr[2] - '0';

                //Add any relevant module to a cell that is empty (this block of code ignores logic for finding clashes)
                CoreModule *currentCoreModule = timeTable[i][j].nextCoreModule;
                CoreModule *coreModule = malloc(sizeof(coreModule));
                if(currentCoreModule == NULL && timeTable[i][j].available == true){ //if the cell is empty and available for teaching
                    strcpy(coreModule->moduleID, relevantModules[currentRelevantModule].moduleID);
                    coreModule->semester = relevantModules[currentRelevantModule].semester;
                    coreModule->nextCoreModule = NULL;
                    currentCoreModule = coreModule;
                    currentCoreModule->nextCoreModule = timeTable[i][j].nextCoreModule;
                    //Attempt to add one of the modules practical session first
                    if(numberOfPracticals > 0){ //if the module has practicals not timetabled
                        numberOfPracticals--;
                        char pracItoC = (char) (numberOfPracticals + '0');
                        relevantModules[currentRelevantModule].pracAmountAndHr[0] = pracItoC;
                        //append a 'P' onto the module ID so that the user can determine if the slot session is a practical or lecture
                        coreModule->moduleID[7] = 'P';
                        if(j+lengthOfPracticals <= 9){
                            timeTable[i][j].nextCoreModule = coreModule;
                        }
                        for (int k = 0; k < lengthOfPracticals; ++k) {
                            int jIndex = k + 1;
                            timeTable[i][jIndex].nextCoreModule = coreModule;
                        }
                        currentlyTaughtHours += (atoi(&relevantModules[currentRelevantModule].pracAmountAndHr[2]));
                    }
                    //otherwise add the modules lecture
                    else if(numberOfLectures > 0){
                        numberOfLectures--;
                        char lectItoC = (char) (numberOfLectures + '0');
                        relevantModules[currentRelevantModule].lectureAmountAndHr[0] = lectItoC;
                        coreModule->moduleID[7] = 'L';
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

                //IF the cell is not empty AND adding the current relevant module would not result in a timetabling clash
                //AND the teaching slots are permited (according to "times.txt" file) then add the current relevant module
                //then add it to the current timetable cell
                else if(!isClash(timeTable[i][j], relevantModules[currentRelevantModule], schemesList)
                                                && timeTable[i][j].available == true && currentCoreModule != NULL) {
                    strcpy(coreModule->moduleID, relevantModules[currentRelevantModule].moduleID);
                    coreModule->semester = relevantModules[currentRelevantModule].semester;
                    coreModule->nextCoreModule = timeTable[i][j].nextCoreModule;
                    //prioritize timetabling practicals
                    if(numberOfPracticals > 0){ //if the module has practicals not timetabled
                        numberOfPracticals--;
                        char pracItoC = (char) (numberOfPracticals + '0');
                        relevantModules[currentRelevantModule].pracAmountAndHr[0] = pracItoC;
                        coreModule->moduleID[7] = 'P';
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
                        coreModule->moduleID[7] = 'L';
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

    //PRINT THE TIMETABLE//
    char* t9 = "9:00";
    char* t10 = "10:00"; char* t11 = "11:00"; char* t12 = "12:00"; char* t13 = "13:00"; char* t14 = "14:00"; char* t15 = "15:00"; char* t16 = "16:00"; char* t17 = "17:00";
    printf("\n\n %50.50s %90.90s %90.90s %90.90s %90.90s %90.90s %90.90s %90.90s %90.90s \n", t9, t10, t11, t12, t13, t14, t15, t16, t17);
    for (int m = 0; m < 8; ++m) {
        printf("%104.104s","__ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __");
    }

    for (int i = 0; i <  7; i++){
        printf("\n");
        switch(i){
            case 0:
                printf("\n\nMon\t\t  ");
                break;
            case 1:
                printf("\n\nTue\t\t  ");
                break;
            case 2:
                printf("\n\nWed\t\t  ");
                break;
            case 3:
                printf("\n\nThu\t\t  ");
                break;
            case 4:
                printf("\n\nFri\t\t  ");
                break;
            case 5:
                printf("\n\nSat\t\t  ");
                break;
            case 6:
                printf("\n\nSun\t\t  ");
                break;
            default:break;
        }
        for (int j = 0; j < 9; j++) {
            CoreModule *coreModuleToPrint = timeTable[i][j].nextCoreModule;
            char * allModulesToPrint = malloc(((sizeof(char *)+1)*7));
            while(coreModuleToPrint != NULL){
                strcat(allModulesToPrint, coreModuleToPrint->moduleID);
                strcat(allModulesToPrint, " ");
                coreModuleToPrint = coreModuleToPrint->nextCoreModule;
            }
            printf("|");
            printf("%90.90s", allModulesToPrint);
        }
    }
    printf("\n");
}