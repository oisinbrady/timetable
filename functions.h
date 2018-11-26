typedef struct Module{
    char moduleID[7];
    int semester;
    char lectureAmountAndHr[4];
    char pracAmountAndHr[4];
} module;


struct Module * readModules(void);
void menuLoop(void);