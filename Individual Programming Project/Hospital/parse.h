

#ifndef ZAL1IPP_PARSE_H
#define ZAL1IPP_PARSE_H

#include <stdbool.h>
#include "structure.h"

char* readCommand(char *string, bool *eof);

extern void executeCommand(char command[MAX_LINE], List *patientsList, DiseaseList
*diseaseList, bool diagnostic);


#endif //ZAL1IPP_PARSE_H
