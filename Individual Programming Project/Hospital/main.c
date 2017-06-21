#include <stdio.h>
#include <string.h>
#include "parse.h"



int main(int argc, char *argv[]) {
    char command[MAX_LINE];
    List patientsList = NULL;
    DiseaseList diseaseList = NULL;
    bool eof = false;
    bool diagnostic = false;
    int l = 1;

    if (argc > 1){
        if(strcmp(argv[1], "-v") == 0 ){
            diagnostic = true;
        }
        else {
            printf("ERROR");
            return 1;
        }
    }


    while(!eof) {
        readCommand(command, &eof);
        executeCommand(command, &patientsList, &diseaseList, diagnostic);
    }


    return 0;
}