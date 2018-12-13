#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "functions.h"

/**
 * initialise the clash array if it has not already been. This will copy all core modules from the accepted scheme into
 * the clash array (N.B. this is a global array in the program). The function will not add the module ID that the user
 * entered into the program into the clash array.
 * @param scheme the current scheme fed into the function
 * @param semester the current semester of the module
 * @param moduleID the ID (name) of the module
 */
void initialiseClashArray(Scheme scheme, int semester, const char *moduleID) {
    CoreModule *current = scheme.coreModule; //pointer to the current core module
    //create an array which holds all found clashes

    int clashArrayIndex = 0;
    for (int i = 0; i < scheme.numberOfCoreModules; ++i) {
        if (strncmp(moduleID, current->moduleID, 7) != 0){
            if(current->semester == semester){ //add to clash array if core module is in the same semester as the entered module
                strcpy(clashArray[clashArrayIndex], current->moduleID);
                clashArrayIndex++;
            }
        }
        current = current->nextCoreModule;
    }
}

/**
 * updates the clash array by checking if the current module is part of a scheme. If this is the case, any module from the
 * scheme found that are not already in the clash array are added to the clash array by calling addClash().
 * @param *schemesList the array of schemes
 * @param *moduleID the name of the current module that the user has entered
 * @param semester the semester of the current module that the user has entered
 * @param numberOfStudents the number of students that attend the module in which the user entered
 * @param clashArrayInit this is the boolean value which determines if the clash array has already been initialised or not
 */
int updateClashArray(const Scheme *schemesList, const char *moduleID, int semester, int numberOfStudents, bool clashArrayInit) {
    for(int i = 0; i < numberOfSchemes; i++) {
        CoreModule *currentCoreModule = schemesList[i].coreModule;
        CoreModule *clashes = malloc(sizeof(clashes));
        const char * checkIfEmpty = "0";
        //in the event that the schemesList file is corrupt causing the current scheme to be empty
        if(strncmp(schemesList[i].schemeCode , checkIfEmpty,1) == 0){
            return numberOfStudents;
        }

        for (int j = 0; j < schemesList[i].numberOfCoreModules ; ++j) {
            if(strncmp(moduleID, currentCoreModule->moduleID,7) == 0) { //find any schemes with the entered moduleID
                numberOfStudents += schemesList[i].numberOfStudents;
                Scheme currentScheme = schemesList[i];
                if(!clashArrayInit){
                    initialiseClashArray(currentScheme, semester, moduleID);
                    clashArrayInit = true;
                    currentCoreModule = currentCoreModule->nextCoreModule;
                    continue;
                }
                //if it has been determined that the module in question DOES clash then add it to the clash array
                //by calling this function
                addClash(currentScheme, semester, moduleID);
            }
            currentCoreModule = currentCoreModule->nextCoreModule;
        }
    }
    return numberOfStudents;
}

/**
 * this will add a clashing module to the array of clashing modules
 */
void addClash(Scheme scheme, int semester, const char *moduleID){
    CoreModule *current;
    current = scheme.coreModule;//current points to start of core modules list
    bool clashAlreadyRecorded = false;
    //iterate to the end of the linked list
    for(int i = 0; i < scheme.numberOfCoreModules; i++) {
        for (int j = 0; j < scheme.numberOfCoreModules; ++j) {
            //check if the current module is already recorded as a clash
            for (int k = 0; k < sizeof(clashArray)/ sizeof(clashArray[0]); ++k) {
                if (strncmp(current->moduleID, clashArray[k], 7) == 0) {
                    clashAlreadyRecorded = true;
                    break;
                }
            }
            //add the clashing module to the next available space in the clashes array
            if (!clashAlreadyRecorded && current->semester == semester && (strncmp(current->moduleID, moduleID , 7) != 0) ) {
                for (int l = 0; l < sizeof(clashArray); ++l) { //find the next free slot in the clash array
                    if (strncmp("\000", clashArray[l], 7) == 0) {
                        strncpy((char *) clashArray[l], current->moduleID, 8);
                        break;
                    }
                }
            }else{
                clashAlreadyRecorded = false;
            }
        }
        current = current->nextCoreModule;
    }
}

/**
 * This will print out all information regarding a module that the user entered onto the CLI
 * @param modulesList the array of modules parsed from the "modules.txt" file
 * @param teachingTimes the 2D array of available teaching times in a week parsed from the "times.txt" file
 * @param schemesList the array of schemes parsed from the "schemes.txt" file
 */
void moduleInfo(Module * modulesList, int ** teachingTimes, Scheme * schemesList){
    char moduleID[7];
    int semester = 0;
    int numberOfStudents = 0;
    printf("Enter a module code: ");
    scanf("%s", moduleID);
    bool moduleExists = false;
    //find if the module entered actually exists
    for (int i = 0; i < numberOfModules; ++i) {
        if(strncmp(moduleID, modulesList[i].moduleID, 7) == 0){
            moduleExists = true;
            break;
        }
    }
    //if the module does exist print out its relevant information
    if(moduleExists){
        for(int i = 0; i < numberOfModules; i++){
            if(strncmp(moduleID, modulesList[i].moduleID, 7) == 0){
                printf("Semester: %d\n",modulesList[i].semester);
                printf("Number of lectures: %.1s \n", modulesList[i].lectureAmountAndHr);
                printf("Length of lectures: %s \n", modulesList[i].lectureAmountAndHr + strlen(modulesList[i].lectureAmountAndHr) - 1);
                printf("Number of practicals: %.1s \n", modulesList[i].pracAmountAndHr);
                printf("Length of practicals: %s \n", modulesList[i].pracAmountAndHr + strlen(modulesList[i].pracAmountAndHr) - 1);
                semester = modulesList[i].semester;
                break;
            }
        }

        bool clashArrayInit = false;
        //find the number of students that are sitting the entered module and update the clash array
        numberOfStudents = updateClashArray(schemesList, moduleID, semester, numberOfStudents, clashArrayInit);
        printf("Number of students: %d \n", numberOfStudents);
        printf("Clashing modules: ");
        //print out all modules which will clash and thus CANNOT be on the same timetable slot as each other
        for(int i = 0; i< sizeof(clashArray)/ sizeof(clashArray[0]); i++){
            printf("%s ", clashArray[i]);
        }
        printf("\n");
    }
    else{
        printf("This module does not exist (this is a case sensitive search)\n");
    }

    //print out a table of all available teaching times
    //if a slot is availabe its marked with the value 1; else 0.
    printf("\nAvailable slots for teaching on a particular day are indicated by 1\n");
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
            default:break;
        }
        for (int j = 0; j < 9; j++) {
            printf("\t  %d\t", teachingTimes[i][j]);
        }
    }
    printf("\n");
}

