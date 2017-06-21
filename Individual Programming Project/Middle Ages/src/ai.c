/** @file
 *
 *  Source file of AI.
 *  Contains all necessary functions to genrate all actions during a turn.
 *  Some of these funtions can be exported and used in engine (both modules
 *  are independent).
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "ai.h"

#define NORTHWEST 21
#define NORTH 22
#define NORTHEAST 23
#define EAST 24
#define SOUTHEAST 25
#define SOUTH 26
#define SOUTHWEST 27
#define WEST 28

typedef struct {
    int X;
    int Y;
} Vector;

typedef struct NodeListDef NodeList;

struct NodeListDef {
    Piece piece;
    NodeList *next;
};


TreeNode *findMyNextPeasant(TreeNode **tree, int myNumber, int
turns) {

    int myPeasantSymbol = assignSymbol(myNumber, true, 'C');
    TreeNode *found;

    if(*tree == NULL) {
        found = NULL;
    }
    else if((**tree).piece.ch == myPeasantSymbol) {
        found = *tree;
    }
    else {
        found = findMyNextPeasant(&(*tree)->left, myNumber, turns);
        if(found == NULL) {
            found = findMyNextPeasant(&(*tree)->right, myNumber,turns);
        }
    }

    return found;
}

TreeNode *findMyNextNotMovedKnight(TreeNode **tree, int myNumber) {

    int myKnightSymbol = assignSymbol(myNumber, true, 'R');
    TreeNode *found;

    if(*tree == NULL) {
        found = NULL;
    }
    else if((**tree).piece.ch == myKnightSymbol && !(**tree).piece.moved) {
        found = *tree;
    }
    else {
        found = findMyNextNotMovedKnight(&(*tree)->left, myNumber);
        if(found == NULL) {
            found = findMyNextNotMovedKnight(&(*tree)->right, myNumber);
        }
    }

    return found;
}

TreeNode *findKing(TreeNode **tree, int myNumber, bool my) {

    int kingSymbol = assignSymbol(myNumber, my, 'K');
    TreeNode *found;

    if(*tree == NULL) {
        found = NULL;
    }
    else if((**tree).piece.ch == kingSymbol) {
        found = *tree;
    }
    else {
        found = findKing(&(*tree)->left, myNumber, my);
        if(found == NULL) {
            found = findKing(&(*tree)->right, myNumber, my);
        }
    }

    return found;
}

void printAllPieces(TreeNode *tree) {
    if(tree != NULL) {
        fprintf(stderr, "\t%c: x = %d, y = %d turnMoved = %d\n", tree->piece.ch,
                tree->piece.x, tree->piece.y, tree->piece.turnMoved);
        printAllPieces(tree->left);
        printAllPieces(tree->right);
    }
}

bool thisPieceIsMine(int myNumber, char symbol) {

    return (myNumber == 1 &&
            (symbol == 'K' || symbol == 'R' || symbol == 'C'))
           ||
           (myNumber == 2 &&
            (symbol == 'k' || symbol == 'r' || symbol == 'c'));
}

bool itsCloserThanClosest(TreeNode *tree, TreeNode *closest, Piece myPiece,
                          int myNumber) {

    if(closest == NULL) {

        return !thisPieceIsMine(myNumber, tree->piece.ch);
    }
    else if (thisPieceIsMine(myNumber, tree->piece.ch)){

        return false;
    }
    else {

        return metricMin(tree->piece.x, tree->piece.y, myPiece.x, myPiece.y) <
               metricMin(closest->piece.x, closest->piece.y, myPiece.x, myPiece.y);
    }
}

TreeNode *closestEnemy(TreeNode *tree, Piece myPiece,
                       TreeNode *closest, int myNumber) {

    if(tree != NULL) {
        if(itsCloserThanClosest(tree, closest, myPiece, myNumber)) {
            closest = tree;
        }
        closest = closestEnemy((*tree).left, myPiece, closest,
                               myNumber);
        closest = closestEnemy((*tree).right, myPiece, closest,
                               myNumber);
    }

    return closest;
}

NodeList *createMyUnitsList(TreeNode **tree, char symbol, int myNumber,
                            int turns) {

    assert(symbol == 'C' || symbol == 'R');
    NodeList *myUnitsList = NULL;
    TreeNode *myUnit;

    while((myUnit = symbol == 'R' ? findMyNextNotMovedKnight(tree, myNumber)
                                  : findMyNextPeasant(tree, myNumber, turns))) {
        NodeList *newNode = (NodeList*) malloc(sizeof(NodeList));
        newNode->piece = myUnit->piece;
        newNode->next = myUnitsList;
        *tree = deleteNode(newNode->piece, tree);
        myUnitsList = newNode;
    }

    return myUnitsList;
}

int listSize(NodeList *list) {
    int length = 0;

    while(list != NULL) {
        list = list->next;
        length++;
    }

    return length;
}

int defineDirection(Piece from, Piece to) {
    Vector vector;
    vector.X = to.x - from.x;
    vector.Y = to.y - from.y;

    if(vector.X == 0) {
        assert(vector.Y != 0);
        return vector.Y < 0 ? NORTH : SOUTH;
    }
    else if(vector.Y == 0) {
        return vector.X < 0 ? WEST : EAST;
    }
    else if(vector.X > 0 && vector.Y > 0) {
        return SOUTHEAST;
    }
    else if(vector.X < 0 && vector.Y > 0) {
        return SOUTHWEST;
    }
    else if(vector.X < 0 && vector.Y < 0) {
        return NORTHWEST;
    }
    else{
        assert(vector.X > 0 && vector.Y < 0);
        return NORTHEAST;
    }
}

int findBestDirection(int previousDirection, int *i) {
    int newDirection;

    if(*i % 2 != 0) {
        newDirection = previousDirection + *i;
    }else{
        newDirection = previousDirection - *i;
    }

    if(newDirection < 21) {
        newDirection = 29 - (21 - newDirection);
    }else if(newDirection > 28) {
        newDirection = 20 + (newDirection - 28);
    }

    (*i)++;
    return newDirection;
}

bool isOnPiecesList(int newX, int newY, NodeList *myPiecesList) {

    while(myPiecesList != NULL) {
        if(myPiecesList->piece.x == newX && myPiecesList->piece.y == newY) {
            return true;
        }else{
            myPiecesList = myPiecesList->next;
        }
    }

    return false;
}

void giveNewCoordinates(int *newX, int *newY, Piece piece, int direction,
                        int n) {
    switch (direction) {
        case NORTH:
            if(piece.y - 1 > 0) {
                *newX = piece.x;
                *newY = piece.y - 1;
            }
            break;
        case SOUTH:
            if(piece.y + 1 <= n) {
                *newX = piece.x;
                *newY = piece.y + 1;
            }
            break;
        case WEST:
            if(piece.x - 1 > 0) {
                *newX = piece.x - 1;
                *newY = piece.y;
            }
            break;
        case EAST:
            if(piece.x + 1 <= n) {
                *newX = piece.x + 1;
                *newY = piece.y;
            }
            break;
        case NORTHWEST:
            if(piece.x - 1 > 0 && piece.y - 1 > 0) {
                *newX = piece.x - 1;
                *newY = piece.y - 1;
            }
            break;
        case NORTHEAST:
            if(piece.x + 1 <= n && piece.y - 1 > 0) {
                *newX = piece.x + 1;
                *newY = piece.y - 1;
            }
            break;
        case SOUTHWEST:
            if(piece.x - 1 > 0 && piece.y + 1 <= n) {
                *newX = piece.x - 1;
                *newY = piece.y + 1;
            }
            break;
        case SOUTHEAST:
            if(piece.x + 1 <= n && piece.y + 1 <=n) {
                *newX = piece.x + 1;
                *newY = piece.y + 1;
            }
            break;
        default:
            assert(true);
            break;
    }
}

Piece tryToCreateNewPiece(TreeNode **tree, NodeList **myPeasantsList,
                          bool createPeasant, int direction, int n, int turns,
                          int myNumber) {
    int newX = 0, newY = 0;
    int symbol = createPeasant ? 'C' : 'R';
    Piece piece = (*myPeasantsList)->piece;

    giveNewCoordinates(&newX, &newY, piece, direction, n);

    if(newX != 0 && newY != 0 && findNode(newX, newY, *tree) == NULL &&
            !isOnPiecesList(newX, newY, *myPeasantsList)) {

        //Inserting peasant (who created new piece) from list to tree and freeing
        //the list.
        NodeList *cpy = *myPeasantsList;
        cpy->piece.turnMoved = turns;
        *myPeasantsList = (**myPeasantsList).next;
        *tree = insertElement(*tree, NULL, 0, cpy->piece);
        free(cpy);

        return createPiece(newX, newY, (char) symbol, turns, true, myNumber);
    }

    return createPiece(0, 0, (char) symbol, turns, true, myNumber);
}

TreeNode *producePieceInBestPossibleSquare(TreeNode **tree,
                                           NodeList **myPeasantsList,
                                           bool producePeasant, int direction,
                                           int n, int turns, int myNumber) {

    Piece myPeasant = (*myPeasantsList)->piece;
    Piece newPiece = tryToCreateNewPiece(tree, myPeasantsList, producePeasant,
                                           direction, n, turns, myNumber);
    int newDirection = direction;
    int count = 1;

    while(newPiece.x == 0 &&
          (newDirection = findBestDirection(newDirection, &count)) !=
                  direction){

        newPiece = tryToCreateNewPiece(tree, myPeasantsList, producePeasant,
                                       newDirection, n, turns, myNumber);

    }

    if(newPiece.x == 0) {
        //Couldn't create new peasant - I need to insert peasant from list to
        // tree and free myPeasantList, because loop above didn't do it.
        NodeList *cpy = *myPeasantsList;
        *myPeasantsList = (**myPeasantsList).next;
        *tree = insertElement(*tree, NULL, 0, cpy->piece);
        free(cpy);
    }
    else{
        //Inserting new piece to tree.
        *tree = insertElement(*tree, NULL, 0, newPiece);
        if(producePeasant) {
            printf("PRODUCE_PEASANT %d %d %d %d\n", myPeasant.x, myPeasant.y,
                   newPiece.x, newPiece.y);
        }else{
            printf("PRODUCE_KNIGHT %d %d %d %d\n", myPeasant.x, myPeasant.y,
                   newPiece.x, newPiece.y);
        }
        fflush(stdout);
    }

    return *tree;
}

TreeNode *produceNewPieces(TreeNode **tree, NodeList **myPeasantsList,
                          bool producePeasant, int n, int turns, int myNum) {

    while(*myPeasantsList != NULL) {
        if((*myPeasantsList)->piece.turnMoved - turns > 2) {


            TreeNode *closest = closestEnemy(*tree,
                                             (*myPeasantsList)->piece,
                                             NULL, myNum);
            int direction = defineDirection((*myPeasantsList)->piece, closest->piece);
            *tree = producePieceInBestPossibleSquare(tree,myPeasantsList,
                                                     producePeasant, direction,
                                                     n, turns, myNum);
        }else{
            //This peasant cannot produce units - I drop it on the tree and
            // delete from list.
            NodeList *cpy = *myPeasantsList;
            *tree = insertElement(*tree, NULL, 0 , (*myPeasantsList)->piece);
            *myPeasantsList = (*myPeasantsList)->next;
            free(cpy);

        }
    }

    return *tree;
}

TreeNode *tryToMoveKnight(TreeNode **tree, NodeList **myNotMovedKnightsList,
                          int direction, int n, int myNumber, bool *win) {

    assert(myNotMovedKnightsList != NULL);
    int newX = 0, newY = 0;
    Piece piece = (*myNotMovedKnightsList)->piece;
    giveNewCoordinates(&newX, &newY, piece, direction, n);
    TreeNode *onNewSquare = findNode(newX, newY, *tree);
    int count = 0;
    int newDirection = direction;
    bool knightBattle = false;

    while(isOnPiecesList(newX, newY, *myNotMovedKnightsList) ||
            (onNewSquare != NULL &&
             (onNewSquare->piece.ch == assignSymbol(myNumber, true, 'K') ||
              onNewSquare->piece.ch == assignSymbol(myNumber, true, 'R') ||
              onNewSquare->piece.ch == assignSymbol(myNumber, true, 'C')))
            && count < 5) {

        newDirection = findBestDirection(newDirection, &count);

        giveNewCoordinates(&newX, &newY, piece, newDirection, n);

        onNewSquare = findNode(newX, newY, *tree);
    }
    if(count < 5) {
        if(onNewSquare != NULL) {
            if(onNewSquare->piece.ch == assignSymbol(myNumber, false, 'R')) {
                knightBattle = true;
            }else if(onNewSquare->piece.ch ==
                    assignSymbol(myNumber, false, 'K')) {
                *win = true;

            }
                *tree = deleteNode(onNewSquare->piece, tree);
        }
        printf("MOVE %d %d %d %d\n", (*myNotMovedKnightsList)->piece.x,
               (*myNotMovedKnightsList)->piece.y, newX, newY);
        fflush(stdout);
        (*myNotMovedKnightsList)->piece.x = newX;
        (*myNotMovedKnightsList)->piece.y = newY;
        (*myNotMovedKnightsList)->piece.moved = true;
    }

    if(!knightBattle) {
        *tree = insertElement(*tree, NULL, 0, (*myNotMovedKnightsList)->piece);
    }
    NodeList *cpy = *myNotMovedKnightsList;
    *myNotMovedKnightsList = (*myNotMovedKnightsList)->next;
    free(cpy);

    return *tree;
}

void deleteList(NodeList **list) {

    if(*list != NULL) {
        while(*list != NULL) {
            NodeList *cpy = *list;
            *list = (*list)->next;
            free(cpy);
        }
    }
}

TreeNode *moveKnights(TreeNode **tree, NodeList **knightsList, int n,
                      int myNumber, bool *win) {

    int direction;

    while(*knightsList != NULL && !*win) {
        direction = defineDirection((*knightsList)->piece,
                                    closestEnemy(*tree, (*knightsList)->piece,
                                                 NULL, myNumber)->piece);
        *tree = tryToMoveKnight(tree, knightsList, direction, n,
                                myNumber, win);
    }

    deleteList(knightsList);

    return *tree;
}

int generateAction(TreeNode **tree, int *turns, bool *myTurn, int myNum,
                   int n) {

    bool win = false;

    NodeList *myKnightsToMoveList = createMyUnitsList(tree, 'R', myNum, *turns);
    *tree = moveKnights(tree, &myKnightsToMoveList, n, myNum, &win);

    if(!win) {
        NodeList *myPeasantsList = createMyUnitsList(tree, 'C', myNum, *turns);
        //AI to beat: change size of peasantsList to 1.
        *tree = produceNewPieces(tree, &myPeasantsList,
                                 listSize(myPeasantsList) < 2,
                                 n, *turns, myNum);
    }
    return endTurn(turns, myTurn, myNum, *tree, win);

}