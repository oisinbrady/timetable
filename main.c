#include <stdio.h>
#include "parseFiles.h"
int main() {
    char* folderPath = getFolder();
    Module *modulesList = readModules(folderPath);
    readSchemes(folderPath);
    return 0;
}