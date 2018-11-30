int numberOfModules;
int numberOfSchemes;
typedef struct module{
    char moduleID[7];
    int semester;
    char lectureAmountAndHr[4];
    char pracAmountAndHr[4];
} Module;
typedef struct scheme{
    char schemeCode[4];
    int yearOfStudy;
    int numberOfStudents;
    int numberOfCoreModules;
    char *coreModules; //points to an array of core modules
} Scheme;


char *getFolder();
Module * readModules(char *file);
Scheme * readSchemes(char *file);
void menuLoop(void);