typedef int bool;
#define true 1
#define false 0
int numberOfModules;
int numberOfSchemes;
char clashArray[100][8];
typedef struct module{
    char moduleID[7];
    int semester;
    char lectureAmountAndHr[4];
    char pracAmountAndHr[4];
} Module;
typedef struct coreModule {
    char moduleID[7];
    int semester;
    struct coreModule *nextCoreModule;
} CoreModule;
typedef struct scheme{
    char schemeCode[5];
    int yearOfStudy;
    int numberOfStudents;
    int numberOfCoreModules;
    CoreModule *coreModule; //pointer to the first core module in a linked list (header)
} Scheme;
typedef struct timetableCell{
    bool available; //if the teaching slot (timetable cell) is available or not (based of times.txt / teachingTimes[][])
    struct coreModule *nextCoreModule; //A list of lectures/practicals for a particular day
}TimeTableCell;
char *getFolder(void);
Module * readModules(char *file);
Scheme * readSchemes(char *file, Module *modulesList);
int ** readTimes(char *file); //7 days a week w/ a max of 9 hours teaching/day
void printMenu(void);
void menuLoop(Module *moduleList, Scheme *schemesList, int **teachingTimes);
void moduleInfo(Module * modulesList, int ** teachingTimes, Scheme * schemesList);
void initialiseClashArray(Scheme scheme, int semester, const char *moduleID);
void addClash(Scheme scheme, int semester, const char *moduleID);
void buildTimetable(Module *modulesList, Scheme * schemesList, int ** teachingTimeSlots);