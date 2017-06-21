#include "structure.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 30

const char *commands[MAX_COMMAND_LENGTH] = {
        "NEW_DISEASE_ENTER_DESCRIPTION",
        "NEW_DISEASE_COPY_DESCRIPTION",
        "CHANGE_DESCRIPTION",
        "PRINT_DESCRIPTION",
        "DELETE_PATIENT_DATA"
};


enum command typeOfCommand(const char* const name) {
    int i;
    for (i=NewDiseaseEnterDescription; i!=NoCommand; ++i) {
        if (0 == strcmp(commands[i], name)) {
            return i;
        }
    }
    return i;
}

List addNode(char *patientName) {
    List new = (List) malloc(sizeof(struct Node));
    new->next = NULL;
    new->patientsDiseases = NULL;
    strcpy(new->name, patientName);
    return new;
}

DiseaseList addDiseaseNode(char *description) {
    DiseaseList new = (DiseaseList) malloc(sizeof(struct DiseaseNode));
    new->next = NULL;
    strcpy(new->name, description);
    new->amount = 0;
    return new;
}

DescriptionList addDescriptionNode(DiseaseList diseaseNode) {
    DescriptionList new = (DescriptionList) malloc(sizeof(struct DescriptionNode));
    new->next = NULL;
    new->diseasePtr = diseaseNode;
    return new;
}

void readName(char name[MAX_NAME_LENGTH]) {
    scanf("%s", name);
}

void readNumber(int *number) {
    scanf("%d", &(*number));
}

void readDescription(char description[MAX_DISEASE_DESCRIPTION]) {
    char c = getchar();
    scanf("%[^\n]%*c", description);
}

void addPatient (List *patientsList, char *patientName) {
    List patient = *patientsList;

    if (patient == NULL) {
        patient = addNode(patientName);
        *patientsList = patient;
    } else if (strcmp(patient->name, patientName) != 0) {
        addPatient(&patient->next, patientName);
    }
}

void addDisease(DiseaseList *diseaseList, char *description) {
    DiseaseList disease = *diseaseList;

    if(disease == NULL) {
        disease = addDiseaseNode(description);
        *diseaseList = disease;
    }else {
        addDisease(&disease->next, description);
    }
}

void reversePatientsDiseasesList(DescriptionList *patientsDiseases) {

    if (patientsDiseases == NULL) {
    }else {
        DescriptionList tmp = *patientsDiseases;
        DescriptionList help;
        while (tmp->next != NULL) {
            help = tmp->next;
            tmp->next = help->next;
            help->next = *patientsDiseases;
            *patientsDiseases = help;
        }
    }
}

void addDiseaseToPatient(DiseaseList *diseaseList, List *patientsList,
                         char *patientName, char* description) {
    DiseaseList disease = *diseaseList;
    List patient = *patientsList;

    while(disease->next != NULL) {
        disease = disease->next;
    }
    while(strcmp(patient->name, patientName)) {
        patient = patient->next;
    }

    DescriptionList nowy = (DescriptionList) malloc(sizeof(struct DescriptionNode));
    nowy->next = patient->patientsDiseases;
    nowy->diseasePtr = disease;
    patient->patientsDiseases = nowy;
    patient->patientsDiseases->diseasePtr->amount++;

    reversePatientsDiseasesList(&patient->patientsDiseases);
}

void newDiseaseEnterDescription(List *patientsList, DiseaseList
*diseaseList, char *patientName) {
    List patient = *patientsList;
    DiseaseList disease = *diseaseList;
    char description[MAX_DISEASE_DESCRIPTION];

    addPatient(&patient, patientName);
    *patientsList = patient;

    readDescription(description);
    addDisease(&disease, description);
    *diseaseList = disease;

    addDiseaseToPatient(&disease, &patient, patientName, description);

    printf("OK\n");
}

void newDiseaseCopyDescriptionNewPatient(List *patient1, DescriptionList
patient2Diseases) {

    (*patient1)->patientsDiseases = addDescriptionNode
            (patient2Diseases->diseasePtr);
    (*patient1)->patientsDiseases->diseasePtr->amount++;
}

void newDiseaseCopyDescriptionExistingPatient(List *patient1, DescriptionList
patient2Diseases) {
    DescriptionList patient1Diseases = (*patient1)->patientsDiseases;

    while(patient1Diseases->next != NULL) {
        patient1Diseases = patient1Diseases->next;
    }
    patient1Diseases->next = addDescriptionNode
            (patient2Diseases->diseasePtr);
    patient1Diseases->next->diseasePtr->amount++;
}

void newDiseaseCopyDescription(List *patientsList) {
    List patient1 = *patientsList;
    List patient2 = *patientsList;
    char patient1Name[MAX_NAME_LENGTH];
    char patient2Name[MAX_NAME_LENGTH];
    bool dodany = false;
    bool ignored = false;

    readName(patient1Name);
    readName(patient2Name);

    if(patient1 == NULL) {
        patient1 = addNode(patient1Name);
    }else if (patient1->next == NULL) {
        patient1->next = addNode(patient1Name);
        patient1 = patient1->next;
        dodany = true;
    }else {
        while (!dodany && strcmp(patient1->name, patient1Name) != 0) {
            patient1 = patient1->next;
            if (patient1->next == NULL) { // last patient
                patient1->next = addNode(patient1Name);
                patient1 = patient1->next;
                dodany = true;
            }
        }
    }


    while(strcmp(patient2->name, patient2Name)) {
        patient2 = patient2->next;
        if(patient2 == NULL) {
            printf("IGNORED\n");
            ignored = true;
        }
    }

    DescriptionList patient2Diseases;
    if(!ignored){
        patient2Diseases = patient2->patientsDiseases;
        if(patient2Diseases == NULL){
            printf("IGNORED\n");
            ignored = true;
        }
        while(!ignored && patient2Diseases->next != NULL) {
            patient2Diseases = patient2Diseases->next;
        }


        if(!ignored && dodany) {
            newDiseaseCopyDescriptionNewPatient(&patient1, patient2Diseases);
            printf("OK\n");
        }else if(!ignored){
            newDiseaseCopyDescriptionExistingPatient(&patient1,
                                                     patient2Diseases);
            printf("OK\n");
        }
    }

}

void removeDisease(DiseaseList *diseaseList, DiseaseList diseaseToRemove){

    DiseaseList disease = *diseaseList;
    DiseaseList tmp;
    bool end = false;

    if(disease == diseaseToRemove){
        if(disease->next == NULL) {
            free(disease);
            *diseaseList = NULL;
        }else {
            tmp = (*diseaseList)->next;
            free(disease);
            (*diseaseList) = tmp;
        }
    }
    else{
        while (!end && disease->next != NULL) {
            if (disease->next == diseaseToRemove){
                tmp = disease->next;
                disease->next = disease->next->next;
                free(tmp);
                end = true;
            }
            disease = disease->next;
        }
    }
}

void changeDescription(List *patientsList, DiseaseList *diseaseList) {
    char name[MAX_NAME_LENGTH];
    int number = 1;
    char description[MAX_DISEASE_DESCRIPTION];
    int i = 1;
    List patient = *patientsList;
    DiseaseList disease = *diseaseList;

    readName(name);
    readNumber(&number);
    readDescription(description);

    while(strcmp(patient->name, name)) {
        patient = patient->next;
    }
    if (patient->patientsDiseases == NULL) {
        printf("IGNORED\n");
    }
    else{
        DescriptionList patientDiseases = patient->patientsDiseases;
        while(i < number) {
            patientDiseases = patientDiseases->next;
            i++;
        }
        if(i != number) {
            printf("IGNORED\n");
        }else{

            patientDiseases->diseasePtr->amount--;

            if(patientDiseases->diseasePtr->amount <= 0) {
                removeDisease(&disease, patientDiseases->diseasePtr);
                *diseaseList = disease;
            }
            while(disease->next != NULL){
                disease = disease->next;
            }

            disease->next = addDiseaseNode(description);
            patientDiseases->diseasePtr = disease->next;
            disease->next->amount++;
            disease = disease->next;
            printf("OK\n");
        }

    }

}

void printDescription(List *patientsList){
    List patient = *patientsList;
    char name[MAX_NAME_LENGTH];
    int n = 1;
    int diseaseNumber = 1;

    readName(name);
    readNumber(&n);

    while(patient != NULL && strcmp(patient->name, name)) {
        patient = patient->next;
    }
    if(patient == NULL) {
        printf("IGNORED\n");
    }
    else {
        DescriptionList patientsDisease = patient->patientsDiseases;
        while (patientsDisease != NULL && diseaseNumber < n) {
            patientsDisease = patientsDisease->next;
            diseaseNumber++;
        }
        if (patientsDisease == NULL) {
            printf("IGNORED\n");
        }else {
            printf("%s\n", patientsDisease->diseasePtr->name);
        }
    }
}

void deletePatientDataExistingPatient(List *patientPtr, DiseaseList
*diseaseList) {
    List patient = *patientPtr;
    DiseaseList disease = *diseaseList;
    DescriptionList patientsDiseases = patient->patientsDiseases;

    while (patientsDiseases->next != NULL) {
        DescriptionList tmp = patientsDiseases->next;
        patientsDiseases->next = tmp->next;
        tmp->diseasePtr->amount--;
        if(tmp->diseasePtr->amount <= 0) {
            removeDisease(&disease, tmp->diseasePtr);
        }
        free(tmp);
    }


    patientsDiseases->diseasePtr->amount--;
    if(patientsDiseases->diseasePtr->amount <= 0) {
        removeDisease(&disease, patientsDiseases->diseasePtr);
    }
    free(patientsDiseases);
    patient->patientsDiseases = NULL;
}

void deletePatientData(List *patientList, DiseaseList *diseaseList) {
    List patient = *patientList;
    DiseaseList disease = *diseaseList;
    char name[MAX_NAME_LENGTH];

    readName(name);
    while(patient != NULL && strcmp(patient->name, name)){
        patient = patient->next;
    }
    if(patient == NULL) {
        printf("IGNORED\n");
    }else{
        deletePatientDataExistingPatient(&patient, &disease);
        printf("OK\n");
    }
}

