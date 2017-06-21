//
// Created by smutnyjoe on 01.04.16.
//

#ifndef ZAL1IPP_STRUCTURE_H
#define ZAL1IPP_STRUCTURE_H

#define MAX_COMMAND_LENGTH 31
#define MAX_DISEASE_DESCRIPTION 10001
#define MAX_NAME_LENGTH 30

#define MAX_LINE MAX_COMMAND_LENGTH+MAX_NAME_LENGTH+MAX_DISEASE_DESCRIPTION



struct DiseaseNode;

typedef struct DiseaseNode* DiseaseList;

struct DiseaseNode {
    char name[MAX_DISEASE_DESCRIPTION];
    int amount;
    DiseaseList next;
};

struct DescriptionNode;

typedef struct DescriptionNode* DescriptionList;

struct DescriptionNode {
    DiseaseList diseasePtr;
    DescriptionList next;
};

//Definition of normal list, used to store patients.
struct Node;

typedef struct Node* List;

struct Node {
    char name[MAX_NAME_LENGTH];
    DescriptionList patientsDiseases;
    List next;
};



enum command
{
    NewDiseaseEnterDescription,
    NewDiseaseCopyDescription,
    ChangeDescription,
    PrintDescription,
    DeletePatientData,
    /* marker */
            NoCommand
};

extern const char *commands[MAX_COMMAND_LENGTH];

extern enum command typeOfCommand(const char* const name);

extern List addNode(char *patientName);

extern DiseaseList addDiseaseNode(char *description);

extern DescriptionList addDescriptionNode(DiseaseList diseaseNode);

extern void readName(char name[MAX_NAME_LENGTH]);

extern void readDescription(char description[MAX_DISEASE_DESCRIPTION]);

extern void addDisease(DiseaseList *d, char *diseaseName);

extern void newDiseaseEnterDescription(List *patientsList, DiseaseList
*diseaseList, char *patientName);

extern void newDiseaseCopyDescription(List *patientsList);

extern void changeDescription(List *patientsList, DiseaseList *diseaseList);

extern void printDescription(List *patient);

extern void deletePatientData(List *patientList, DiseaseList *diseaseList);

#endif //ZAL1IPP_STRUCTURE_H
