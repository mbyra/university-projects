/**
 * @file Source code of parser. Contains all functions used for parsing
 * commands and checking if input is in proper format (but does not check if
 * values are correct).
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Command {
    char name[16];
    int data[7];
} Command;


bool checkEnd() {
    int c = getchar();
    if (c != '\n') {
        return false;
    }
    return true;
}

bool checkSpaceAndNumber() {
    int c = getchar();
    int c2;
    if(c != ' ') {
        return false;
    }
    c2 = getchar();
    if(!isdigit(c2)) {
        return false;
    }
    else{
        ungetc(c2, stdin);
        return true;
    }
}

bool checkLetters() {
    int c = getchar();
    if(c != 'I' && c != 'M' && c != 'P' && c != 'E') {
        return false;
    }
    ungetc(c, stdin);
    return true;

}

Command *errCommand(Command **new_command, bool null) {

    Command* errCommand = (Command*) malloc(sizeof(struct Command));
    strcpy(errCommand->name, "ERROR");
    if(!null) {
        free(*new_command);
    }

    return errCommand;
}

Command *parseCommand() {

    if(!checkLetters()) {
        return errCommand(NULL, true);
    }

    Command* newCommand = (Command*) malloc(sizeof(struct Command));

    if(scanf("%s", newCommand->name) == EOF) {
        return errCommand(&newCommand, false);
    };

    int i = 0;
    if(strcmp(newCommand->name, "END_TURN") == 0) {
        if(!checkEnd()) {
            return errCommand(&newCommand, false);
        }
    }else{
        if(!checkSpaceAndNumber()) {
            return errCommand(&newCommand, false);
        }
    }


    if (strcmp(newCommand->name, "INIT") == 0) {

        for(i = 0; i < 7; i++) {
            if(scanf("%d", &newCommand->data[i]) == EOF ||
               (i < 6 && !checkSpaceAndNumber()) ||
               (i == 6 && !checkEnd())) {

                return errCommand(&newCommand, false);
            }
        }

    }
    else if (strcmp(newCommand->name, "MOVE") == 0 ||
            strcmp(newCommand->name, "PRODUCE_KNIGHT") == 0 ||
            strcmp(newCommand->name, "PRODUCE_PEASANT") == 0){

        for(i = 0; i < 4; i++) {
            if(scanf("%d", &newCommand->data[i]) == EOF ||
               (i < 3 && !checkSpaceAndNumber()) ||
               (i == 3 && !checkEnd())) {

                return errCommand(&newCommand, false);
            }
        }
    }

    return newCommand;
}
