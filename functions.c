#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "functions.h"

Module * readModules(void){
    int maxInputSize = 100;
    char *fileDirectory = malloc((size_t) maxInputSize);
    printf("Enter the directory name of timetable files: ");
    scanf("%s", fileDirectory);
    //create string directory path for modules.txt
    char *modules = "";
    if((modules = malloc(strlen(fileDirectory)+strlen(modules)+1)) != NULL){
        modules[0] = '\0';   // ensures the memory is an empty string
        modules = strcat(fileDirectory,"/modules.txt"); //TODO this will be OS specific - but program tested on linux environment so change to '/modules.txt'
    } else {
        perror("malloc failed!\n");
        exit(-1);
    }
    //attempt to open the modules.txt file in the user specified directory path
    FILE *file = fopen(modules, "r");
    if(file == NULL) {
        perror("Error opening file");
        exit(-1);
    }
    //each module has all four variables:
    char moduleID[8];
    int semester;
    char lectureAmountAndHr[4];
    char pracAmountAndHr[4];
    //find the number of modules (lines) in the file
    int numberOfModules = 0;
    while(!feof(file)){
        int ch =  fgetc(file);
        if(ch == '\n'){
            numberOfModules++;
        }
    }
    rewind(file); //go back to the beginning of the file
    //create a dynamic array whose size is based of the numberOfModules currently added
    Module *listOfModules = calloc((size_t) numberOfModules, (sizeof(Module) + 3)); //TODO the X * operation is not increasing the size only the module size is. the size is always 8 only has 8 values: this is why only 8 modules are printed
    int i = sizeof(listOfModules);
    printf("%d", (int) sizeof(listOfModules));
    //iterate over each line in modules.txt to record each module
    int moduleIterator = 0;
    while(fscanf(file, "%s %d %s %s\n", moduleID, &semester, lectureAmountAndHr, pracAmountAndHr)!= EOF){
        //allocate memory for a new instance of module
        Module *module = malloc(sizeof(*module) + 3); //+ 3 for the 3 terminating characters in each char sequence
        strcpy(module->moduleID, moduleID);
        module->semester = semester;
        strcpy(module->lectureAmountAndHr, lectureAmountAndHr);
        strcpy(module->pracAmountAndHr, pracAmountAndHr);
        //add the instantiated struct to an array of structs
        listOfModules[moduleIterator] = *module;
        //listTest[moduleIterator - 1] = module;
        moduleIterator++;
        //reallocate the array so more pointers to modules can be added (double its size)
        //if(moduleIterator > (int) sizeof(listOfModules)){
          //  listOfModules = realloc(listOfModules, (sizeof(Module) * 2));
        //}
        free(module);
    }
    free(fileDirectory);
    return listOfModules;
}