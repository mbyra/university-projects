#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "structure.h"

#define MAX_NAME_LENGTH 30

void readCommand(char *string, bool *eof) {

    if (scanf("%s", string) == EOF) {
        *eof = true;
    }
}


void executeCommand(char command[MAX_LINE], List *patientsListPtr, DiseaseList
*diseaseListPtr, bool diagnostic) {
    List patientList = *patientsListPtr;
    DiseaseList diseaseList = *diseaseListPtr;
    char patientName[MAX_NAME_LENGTH];
    bool first = patientList == NULL;
    int i = 0;

    switch(typeOfCommand(command)) {
        case NewDiseaseEnterDescription:
            readName(patientName);
            newDiseaseEnterDescription(&patientList, &diseaseList, patientName);
            if (first){
                *patientsListPtr = patientList;
                *diseaseListPtr = diseaseList;
            }
            break;
        case NewDiseaseCopyDescription:
            newDiseaseCopyDescription(&patientList);
            break;
        case ChangeDescription:
            changeDescription(&patientList, &diseaseList);
            (*diseaseListPtr) = diseaseList;
            break;
        case PrintDescription:
            printDescription(&patientList);
            break;
        case DeletePatientData:
            deletePatientData(&patientList, &diseaseList);
            break;
        default:
            assert(true);
            break;
    }

    while(diseaseList != NULL) {
        i++;
        diseaseList = diseaseList->next;
    }
    fprintf(stderr, "DESCRIPTIONS: %d\n", i);
}