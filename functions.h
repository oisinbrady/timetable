typedef int bool;
#define true 1
#define false 0
int numberOfModules;
int numberOfSchemes;
typedef struct module{
    char moduleID[7]; //TODO increment these char arrays to account for terminating character?
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
char clashArray[100][8];

char *getFolder();
Module * readModules(char *file);
Scheme * readSchemes(char *file, Module *modulesList);
int ** readTimes(char *file); //7 days a week w/ a max of 9 hours teaching/day

void buildTimetable(Module *modulesList, Scheme * schemesList, int ** teachingTimeSlots);
void moduleInfo(Module * modulesList, Scheme * schemesList);
void initialiseClashArray(Scheme scheme, int semester, const char *moduleID);
void addClash(Scheme scheme, int semester, const char *moduleID);