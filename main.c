#include <stdio.h>
#include "functions.h"
int main() {
    char* folderPath = getFolder();
    Module *modulesList = readModules(folderPath);
    readSchemes(folderPath);
    return 0;
}