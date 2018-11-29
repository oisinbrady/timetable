typedef struct module{
    char moduleID[7];
    int semester;
    char lectureAmountAndHr[4];
    char pracAmountAndHr[4];
} Module;
typedef struct schemes{
    char schemeCode[4];
    int yearOfStudy;
    int numberOfStudents;
    int numberOfCoreModules;

} Schemes;

int numberOfModules;

char *getFolder();
Module * readModules(char *file);
void menuLoop(void);