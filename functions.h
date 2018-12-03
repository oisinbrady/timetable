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
    struct coreModule *nextCoreModule;
} CoreModule;

typedef struct scheme{
    char schemeCode[5];
    int yearOfStudy;
    int numberOfStudents;
    int numberOfCoreModules;
    CoreModule *coreModule; //pointer to the first core module in a linked list (header)
} Scheme;


char *getFolder();
Module * readModules(char *file);
Scheme * readSchemes(char *file);
void push(CoreModule *header, char *moduleID);
void menuLoop(void);