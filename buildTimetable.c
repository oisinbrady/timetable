#include <stdio.h>
#include <malloc.h>
#include "functions.h"

void buildTimetable(Module *modulesList, Scheme * schemesList, int ** teachingTimeSlots){
    int semester;
    printf("\nEnter the semester for the timetable (1/2): ");
    scanf("%d", &semester);
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
    //Code here

    //TODO iterate over modulesList and place any module that is in the same semester and yearOfStudy as uSemester and uYear
    //TODO into a new array of modules = modulesArray

    //TODO WHILE(timetable is not full) - get readTime() function to calculate number of teaching hours (int teachingHours) (while(currentHours <= teachingHours))
    //TODO iterate through timetable cells and for each cell with bool value == 1: add next module from modulesArray
    //TODO IF: current timeTableCell's linked list does not contain a module which belongs to the same scheme as next module
    //TODO decrement modulesArray[current] lecture/practical amount accordingly (add all practicals first for a module then lectures)
    //TODO increment index of modulesArray (if currentModulesIndex > modulesArray.length() THEN currentModulesIndex = 0)
    //TODO goto next timetable cell (if currentCellIndex > timetable.length() THEN currentCell = 0)
    //TODO increment the currentHours var by the length of the lec/prac just added (e.g. currentHours += &module.pracAmountAndTime[2])


    //TODO print out each cell's linked list
}