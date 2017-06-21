/**
 * @file
 *
 * Source code of engine of a game. Contains all necessary functions to
 * start, play, check arguments from input, and end a game.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "engine.h"

#define MAXSIZE 2147483647
#define CONTINUE 3
#define INPUT_ERROR 42
#define DRAW 1
#define DRAW_AFTER_END_TURN 5
#define END_TURN 6
#define P1_WIN 7
#define P2_WIN 8
#define WIN 0
#define LOST 2
#define DRAW_BUT_FIRST_PLAYER_READS_LAST_INPUT 9

void deallocateTree(TreeNode *node) {
    if (node != NULL) {
        deallocateTree(node->left);
        deallocateTree(node->right);
        free(node);
    }
}

Piece createPiece(int x, int y, char ch, int turns, bool myTurn, int myNumber) {
    Piece piece;
    piece.x = x;
    piece.y = y;
    piece.ch = ch;
    piece.turnMoved = turns + 1;
    piece.moved = false;

    if((myTurn && myNumber == 1) || (!myTurn && myNumber == 2)) {
        if(ch == 'k') {
            piece.ch = 'K';
        }else if(ch == 'c') {
            piece.ch = 'C';
        }else if(ch == 'r') {
            piece.ch = 'R';
        }
    }else{
        if(ch == 'K') {
            piece.ch = 'k';
        }else if(ch == 'C') {
            piece.ch = 'c';
        }else if(ch == 'R') {
            piece.ch = 'r';
        }
    }

    return piece;
}

TreeNode *createNode(Piece piece) {
    TreeNode *tmp = (TreeNode*) malloc(sizeof(TreeNode));

    tmp->left = tmp->right = NULL;
    tmp->piece = piece;

    return tmp;
}

int compareNum(int x1, int y1, int x2, int y2) {

    if(x1 > x2) {
        return -1;
    }
    else if(x1 == x2) {
        if(y1 > y2) {
            return -1;
        }else if(y1 == y2) {
            return 0;
        }else {
            return 1;
        }
    }
    else{
        return 1;
    }
}

TreeNode *insertElement(TreeNode *root, char** board, int m, Piece piece) {

    //AI version: added surrounding condition board != NULL.
    if(board != NULL) {
        if (piece.x < m && piece.y < m) {
            board[piece.y][piece.x] = piece.ch;
        }
    }

    if(root == NULL) {
        return createNode(piece);
    }
    else {
        TreeNode *ptr = root;
        TreeNode *prev = NULL;
        bool left = false;

        while(ptr != NULL) {
            prev = ptr;
            if (compareNum(ptr->piece.x, ptr->piece.y, piece.x, piece.y) < 0) {
                ptr = ptr->left;
                left = true;
            }
            else {
                ptr = ptr->right;
                left = false;
            }
        }

        if(left){
            prev->left = createNode(piece);
        }
        else{
            prev->right = createNode(piece);
        }
        return root;
    }
}

//Piece findMin(TreeNode *node) {
//    if(node->left == NULL) {
//        return node->piece;
//    }
//    else{
//        return findMin(node->left);
//    }
//}

Piece findMin(TreeNode *node) {

    while(node->left != NULL) {
        node = node->left;
    }

    return node->piece;
}

TreeNode *deleteNode(Piece piece, TreeNode **node) {

    if(node == NULL) {
        fprintf(stderr, "Trying to remove unexistent element from tree\n");
        assert(true);
        return NULL;
    }
    else if(compareNum((*node)->piece.x, (*node)->piece.y,
                       piece.x, piece.y) < 0) {
        (*node)->left = deleteNode(piece, &(*node)->left);
    }
    else if(compareNum((*node)->piece.x, (*node)->piece.y,
                       piece.x, piece.y) > 0) {
        (*node)->right = deleteNode(piece, &(*node)->right);
    }
    else{
        if((*node)->right != NULL && (*node)->left != NULL) {
            Piece min = findMin((*node)->right);
            (*node)->piece = min;
            (*node)->right = deleteNode(min, &(*node)->right);
        }
        else{
            TreeNode *temp = *node;
            if((*node)->left == NULL) {
                *node = (*node)->right;
            }
            else if((*node)->right == NULL) {
                *node = (*node)->left;
            }
            free(temp);
        }
    }

    return *node;
}

void resetPieces(TreeNode *node, int turns) {

    if(node != NULL) {
        if(node->piece.moved) {
            node->piece.moved = false;
            node->piece.turnMoved = turns;
        }
        resetPieces(node->left, turns);
        resetPieces(node->right, turns);
    }
}

void startGame(TreeNode **tree) {
    *tree = NULL;
}

void printResult(int *result, int myNumber) {

    if(*result == INPUT_ERROR) {
    }else if(*result == DRAW || *result == DRAW_AFTER_END_TURN) {
        *result = DRAW;
    }else if(*result == P1_WIN) {
        if(myNumber == 1) {
            *result = WIN;
        }else {
            *result = LOST;
        }
    }else if(*result == P2_WIN) {
        if(myNumber == 1) {
            *result = LOST;
        }else {
            *result = WIN;
        }
    }else {
        assert(true);
    }

}

void deallocateMemory(char ***board, int m){
    for (int i = 1; i < m; i++) {
        free((*board)[i]);
    }
    free(*board);
}

void endGame(TreeNode **node, int *result, char ***board, int m, int myNumber) {
    deallocateMemory(board, m);
    deallocateTree(*node);
    printResult(result, myNumber);
}

int min(int a, int b) {
    if(a < b) {
        return a;
    }
    else {
        return b;
    }
}

int max(int a, int b) {
    if(a > b) {
        return a;
    }
    else {
        return b;
    }
}

int metric(int x1, int y1, int x2, int y2) {
    return max(abs(x1 - x2), abs(y1 - y2));
}

int metricMin(int x1, int y1, int x2, int y2) {
    return min(abs(x1 - x2), abs(y1 - y2));
}

//TODO Zmienilem z i = 1 na i = 0
char **allocateMemory(int m) {
    char **board = (char**)malloc(m * sizeof(char*));

    for(int i = 1; i < m; i++) {
        board[i] = (char*) malloc(m * sizeof(char));
        for(int j = 1; j < m; j++) {
            board[i][j] = '.';
        }
    }

    return board;
}


//void putTreeOnBoard(TreeNode *node, char **board, int m) {
//    if(node != NULL) {
//        if(node->piece.y < m && node->piece.x < m) {
//            board[node->piece.y][node->piece.x] = node->piece.ch;
//        }
//        putTreeOnBoard(node->left, board, m);
//        if(node->piece.x <= m) {
//            putTreeOnBoard(node->right, board, m);
//        }
//    }
//}

int assignSymbol(int myNumber, bool my, char symbol) {

    int figSymbol = 0;

    if(my) {
        switch (symbol) {
            case 'K':
                figSymbol = myNumber == 1 ? 'K' : 'k';
                break;
            case 'R':
                figSymbol = myNumber == 1 ? 'R' : 'r';
                break;
            case 'C':
                figSymbol = myNumber == 1 ? 'C' : 'c';
                break;
            default:
                assert(true);
                break;
        }
    }
    else {
        switch (symbol) {
            case 'K':
                figSymbol = myNumber == 1 ? 'k' : 'K';
                break;
            case 'R':
                figSymbol = myNumber == 1 ? 'r' : 'R';
                break;
            case 'C':
                figSymbol = myNumber == 1 ? 'c' : 'C';
                break;
            default:
                assert(true);
                break;
        }
    }

    return figSymbol;
}

void putTreeOnBoard(TreeNode *node, char **board, int m) {
    if(node != NULL) {
        if(node->piece.y < m && node->piece.x < m) {
            board[node->piece.y][node->piece.x] = node->piece.ch;
        }
        putTreeOnBoard(node->left, board, m);
        if(node->piece.x <= m) {
            putTreeOnBoard(node->right, board, m);
        }
    }
}

void printTopleft(char **board, int m) {

    for(int i = 1; i < m; i++) {
        for(int j = 1; j < m; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");

}

//TreeNode *findNode(int x, int y, TreeNode *node) {
//    if(node != NULL) {
//        if(compareNum(x, y, node->piece.x, node->piece.y) == 0) {
//            return node;
//        }
//        else if(compareNum(x, y, node->piece.x, node->piece.y) > 0) {
//            return findNode(x, y, node->left);
//        }
//        else {
//            return findNode(x, y, node->right);
//        }
//    }
//    else {
//        return NULL;
//    }
//}


//Iterative way.
TreeNode *findNode(int x, int y, TreeNode *node) {
    while(node != NULL && compareNum(x, y, node->piece.x, node->piece.y) != 0) {
        if(compareNum(x, y, node->piece.x, node->piece.y) > 0) {
            node = node->left;
        }
        else {
            node = node->right;
        }
    }

    return node;
}

int init(int n, int k, int p, int x1, int y1, int x2, int y2, TreeNode **tree,
         int *nMy, int *kMy, int *playerNumber, char **board, int *m) {

    if(n < 9 || n > MAXSIZE || k < 1 || k > MAXSIZE || p < 1 || p > 2 ||
       x1 < 1 || x1 > n - 3 || y1 < 1 || y1 > n || x2 < 1 || x2 > n - 3 ||
       y2 < 1 || y2 > n || metric(x1, y1, x2, y2) < 8) {
        return INPUT_ERROR;
    }

    *nMy = n;
    *kMy = k;
    *playerNumber = p;
    bool myTurn = (p == 1);
    *m = min(n, 10) + 1;

    *tree = insertElement(*tree, board, *m,
                          createPiece(x1, y1, 'K', k, myTurn, p));
    *tree = insertElement(*tree, board, *m,
                          createPiece(x1 + 1, y1, 'C', k, myTurn, p));
    *tree = insertElement(*tree, board, *m,
                          createPiece(x1 + 2, y1, 'R', k, myTurn, p));
    *tree = insertElement(*tree, board, *m,
                          createPiece(x1 + 3, y1, 'R', k, myTurn, p));
    *tree = insertElement(*tree, board, *m,
                          createPiece(x2, y2, 'k', k, !myTurn, p));
    *tree = insertElement(*tree, board, *m,
                          createPiece(x2 + 1, y2, 'c', k, !myTurn, p));
    *tree = insertElement(*tree, board, *m,
                          createPiece(x2 + 2, y2, 'r', k, !myTurn, p));
    *tree = insertElement(*tree, board, *m,
                          createPiece(x2 + 3, y2, 'r', k, !myTurn, p));

    return CONTINUE;
}

int moveOnUnoccupiedSquare(TreeNode *moved, TreeNode **tree, int x, int y,
                           char **board, int m, int turns) {
    Piece cpy = moved->piece;

    if(moved->piece.y < m && moved->piece.x < m) {
        board[moved->piece.y][moved->piece.x] = '.';
    }
    *tree = deleteNode(moved->piece, tree);
    cpy.x = x;
    cpy.y = y;
    cpy.moved = true;
    cpy.turnMoved = turns;
    *tree = insertElement(*tree,  board, m, cpy);

    return CONTINUE;
}

bool checkOwner(Piece piece, bool myTurn, int myNumber, bool moved) {

    bool players1;
    if(myTurn) {
        players1 = myNumber == 1;
    }else {
        players1 = myNumber != 1;
    }

    if(!moved) { // !moved means beaten.
        players1 = !players1;
    }
    return !((players1 &&
              (piece.ch != 'K' &&
               piece.ch != 'C' &&
               piece.ch != 'R')) ||
             (!players1 &&
              (piece.ch != 'k' &&
               piece.ch != 'c' &&
               piece.ch != 'r')));

}

int moveOnOccupiedSquare(TreeNode *moved_node, TreeNode *beaten_node,
                         TreeNode **tree, bool myTurn, int myNumber,
                         char **board, int m, int turns){
    Piece moved = moved_node->piece;
    Piece beaten = beaten_node->piece;

    if (checkOwner(beaten, myTurn, myNumber, false) == false) {

        return INPUT_ERROR;
    }

    if(moved.y < m && moved.x < m) {
        board[moved.y][moved.x] = '.';
    }
    if(beaten.y < m && beaten.x < m) {
        board[beaten.y][beaten.x] = '.';
    }
    *tree = deleteNode(moved, tree);
    *tree = deleteNode(beaten, tree);

    if((moved.ch == 'k' && beaten.ch == 'K') ||
        (moved.ch == 'K' && beaten.ch == 'k')){

        return DRAW;
    }
    else if((moved.ch == 'r' && beaten.ch == 'C') ||
            (moved.ch == 'R' && beaten.ch == 'c') ||
            (moved.ch == 'k' && beaten.ch == 'C') ||
            (moved.ch == 'K' && beaten.ch == 'c')){
        moved.x = beaten.x;
        moved.y = beaten.y;
        moved.moved = true;
        moved.turnMoved = turns;
        *tree = insertElement(*tree, board, m, moved);

        return CONTINUE;
    }
    else if ((moved.ch == 'C' && beaten.ch == 'r') ||
             (moved.ch == 'c' && beaten.ch == 'R') ||
             (moved.ch == 'C' && beaten.ch == 'k') ||
             (moved.ch == 'c' && beaten.ch == 'K')){
        *tree = insertElement(*tree, board, m, beaten);

        return CONTINUE;
    }
    else if (moved.ch == 'k' && beaten.ch == 'R'){
        *tree = insertElement(*tree, board, m, beaten);

        return P1_WIN;
    }

    else if (moved.ch == 'K' && beaten.ch == 'r'){
        *tree = insertElement(*tree, board, m, beaten);

        return P2_WIN;
    }
    else if(moved.ch == 'R' && beaten.ch == 'k'){
        moved.x = beaten.x;
        moved.y = beaten.y;
        moved.moved = true;
        moved.turnMoved = turns;
        *tree = insertElement(*tree, board, m, moved);

        return P1_WIN;

    }
    else if(moved.ch == 'r' && beaten.ch == 'K'){
        moved.x = beaten.x;
        moved.y = beaten.y;
        *tree = insertElement(*tree, board, m, moved);

        return P2_WIN;
    }
    else if((moved.ch == 'C' && beaten.ch == 'c') ||
            (moved.ch == 'c' && beaten.ch == 'C') ||
            (moved.ch == 'R' && beaten.ch == 'r') ||
            (moved.ch == 'r' && beaten.ch == 'R')){
        return CONTINUE;
    }
    else {
        assert(true);
        return INPUT_ERROR;
    }
}

bool checkParams(int x1, int y1, int x2, int y2, int n) {
    return metric(x1, y1, x2, y2) > 1 || x1 < 1 || y1 < 1 || x2 < 1 || y2 < 1 ||
           x1 > n || y1 > n || x2 > n || y2 > n;
}

int move(int x1, int y1, int x2, int y2, int n, bool myTurn, TreeNode **tree,
         char **board, int m, int turns, int myNumber) {


    if(checkParams(x1, y1, x2, y2, n)) {
        return INPUT_ERROR;
    }

    TreeNode *moved = findNode(x1, y1, *tree);
    TreeNode *beaten = findNode(x2, y2, *tree);

    if(moved == NULL || moved->piece.turnMoved == turns ||
            checkOwner(moved->piece, myTurn, myNumber, true) == false) {

        return INPUT_ERROR;
    }

    if(beaten == NULL) {
        return moveOnUnoccupiedSquare(moved, tree, x2, y2, board, m, turns);
    }else {
        return moveOnOccupiedSquare(moved, beaten, tree, myTurn, myNumber,
                                    board, m, turns);
    }


}

int producePiece(char piece, int x1, int y1, int x2, int y2, int n,
                 bool myTurn, int myNumber, TreeNode **tree, char **board,
                 int m, int turns) {

    if(checkParams(x1, y1, x2, y2, n)) {
        fprintf(stderr, "ai madre: params zle w producePiece, daje input "
                "errora\n");
        return INPUT_ERROR;
    }

    TreeNode *foundNode = findNode(x1, y1, *tree);


    if(foundNode == NULL ||
       (foundNode->piece.ch != assignSymbol(myNumber, false, 'C') &&
        foundNode->piece.ch != assignSymbol(myNumber, false, 'R'))){

        return INPUT_ERROR;
    }

    *tree = insertElement(*tree, board, m,
                          createPiece(x2, y2, piece, turns, myTurn, myNumber));

    foundNode->piece.moved = true;
    foundNode->piece.turnMoved = turns;

    return CONTINUE;
}

int produceKnight(int x1, int y1, int x2, int y2, int n, bool myTurn,
                  int myNumber, TreeNode **tree, char** board, int m,
                  int turns) {

    return producePiece('r', x1, y1, x2, y2, n, myTurn, myNumber, tree, board,
                        m, turns);
}

int producePeasant(int x1, int y1, int x2, int y2, int n, bool myTurn,
                   int myNumber, TreeNode **tree, char** board, int m,
                   int turns){

    return producePiece('r', x1, y1, x2, y2, n, myTurn, myNumber, tree, board,
                        m, turns);
}

int endTurn(int *turns, bool *myTurn, int myNumber, TreeNode *tree, bool win) {

    if((*myTurn && myNumber == 2) || (!*myTurn && myNumber == 1)) {
        resetPieces(tree, *turns);
    }
        //Cause now this is AI program doing nothing.
    if (*myTurn && *turns >= 1 && !win) {
        printf("END_TURN\n");
        fflush(stdout);
    }

    if ((*myTurn && myNumber == 2) || (!*myTurn && myNumber == 1)) {
        (*turns)--;
    }
    *myTurn = !*myTurn;

    if(win) {
        return myNumber == 1 ? P1_WIN : P2_WIN;
    }
    else if (*turns <= 0) {
        return DRAW_AFTER_END_TURN;
    }
    else {
        return END_TURN;
    }
}
