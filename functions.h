typedef struct module{
    char moduleID[7];
    int semester;
    char lectureAmountAndHr[4];
    char pracAmountAndHr[4];
} Module;


Module *readModules(void);
void menuLoop(void);