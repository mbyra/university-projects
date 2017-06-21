/** @file
 *  Main funtion of a game.
 *  Includes parse.h to parse commands and engine.h to perform all necessary
 *  commands as well as recognise correct input.
 *  @return 0 if game ends properly(win or draw), or 42 if there was an input
 *  error.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"
#include "engine.h"
#include "ai.h"

int main() {

    TreeNode *tree;
    startGame(&tree);
    int n = 0, m = 0;
    int turns = 0;
    int myNumber;
    Command *newCommand = parseCommand();
    char **board = allocateMemory(min(newCommand->data[0], 10) + 1);
    int result = init(newCommand->data[0], newCommand->data[1],
                  newCommand->data[2], newCommand->data[3],
                  newCommand->data[4], newCommand->data[5],
                  newCommand->data[6], &tree, &n, &turns, &myNumber, board, &m);
    free(newCommand);

    bool myTurn = (myNumber == 1);

    while (result == CONTINUE || result == END_TURN) {

        if (!myTurn) {


            newCommand = parseCommand();


            if (strcmp(newCommand->name, "MOVE") == 0) {
                result = move(newCommand->data[0], newCommand->data[1],
                              newCommand->data[2], newCommand->data[3],
                              n, myTurn, &tree, board, m, turns, myNumber);

            }
            else if (strcmp(newCommand->name, "PRODUCE_KNIGHT") == 0) {
                result = produceKnight(newCommand->data[0], newCommand->data[1],
                                       newCommand->data[2], newCommand->data[3],
                                       n, myTurn, myNumber, &tree, board, m,
                                       turns);
            }
            else if (strcmp(newCommand->name, "PRODUCE_PEASANT") == 0) {
                result = producePeasant(newCommand->data[0],
                                        newCommand->data[1],
                                        newCommand->data[2],
                                        newCommand->data[3],
                                        n, myTurn, myNumber, &tree, board, m,
                                        turns);
            }
            else if (strcmp(newCommand->name, "END_TURN") == 0) {
                result = endTurn(&turns, &myTurn, myNumber, tree, false);
            }
            else {
                //Unexpected command name.
                result = INPUT_ERROR;
            }

            free(newCommand);
        }

        else {
//            To use as a AI doing nothing but ending its turn:
//            result = endTurn(&turns, &myTurn, myNumber, tree, false);

//            To use as AI which attacks not moving player with its knight
//            and creating new knights:
            result = generateAction(&tree, &turns, &myTurn, myNumber, n);

        }

    }

    endGame(&tree, &result, &board, m, myNumber);

    return result;
}

