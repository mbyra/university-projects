/** @file
 *
 *  Interface of game engine.
 *  Contains all functions needed to start, play, read input (and check
 *  parameters), print output and end the game.
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>

#define CONTINUE 3
#define INPUT_ERROR 42
#define DRAW 1
#define DRAW_AFTER_END_TURN 5
#define END_TURN 6
#define P1_WIN 7
#define P2_WIN 8
#define WIN 0
#define LOST 2

/**
 * Struct of Piece.
 *
 * @param x Number of column of a piece.
 * @param y Number of row of a piece.
 * @param ch Char representing a piece('K', 'C', 'R', 'k', 'c' or 'r')
 * @param turnMoved Last turn this piece was moved.
 * @param moved True if the piece was moved last turn.
 */
typedef struct Piece {
    int x;
    int y;
    char ch;
    int turnMoved;
    bool moved;
} Piece;

/**
 * Struct of a tree containing pieces.
 *
 * @param Piece Struct containing parameters of a Piece @see Piece
 * @param left Pointer to left son of the node.
 * @param right Pointer to right son of the node.
 */
typedef struct TreeNode {
    Piece piece;
    struct TreeNode *left, *right;
} TreeNode;


/**
 * Initializes a game. Needed before first INIT.
 *
 * @param[in,out] tree Tree containing nodes with all existing pieces.
 */
void startGame(TreeNode **tree);

/**
 * Frees memory. Needed after finishing game.
 *
 * @param[in,out] tree Tree containing nodes with all existing pieces.
 * @param[out] result Variable keeping result of the game so that proper output
 * can be printed. Also changed to 0 or 42 and returned in main function.
 * @param[in,out] board Array representing top left part of a board.
 * @param[in] m Size of board.
 * @param[in] myNumber Caller player number (1 or 2).
 */
void endGame(TreeNode **tree, int *result, char ***board, int m, int myNumber);

/**
 * @brief Initializes a game with size of a board, number of rounds and
 * positions of kings.
 *
 * One INIT for each player is needed before any other command.
 *
 * @param[in] n Size of a board.
 * @param[in] k Number of turns of each player.
 * @param[in] p Number of player for whom initialization is called.
 * @param[in] x1 Column number of first player's king.
 * @param[in] y1 Row number of first player's king.
 * @param[in] x2 Column number of second player's king.
 * @param[in] y2 Row number of second player's king.
 * @param[in,out] tree Tree containing nodes with all existing pieces.
 * @param[out] nMy Variable keeping the size of board.
 * @param[out] kMy Variable keeping number of turns.
 * @param[in] p1Initialized Information if INIT of the first player has already
 * been called.
 * @param[in] p2Initialized Information if INIT of the second player has
 * already been called.
 * @param[in,out] board Array representing top left part of a board.
 * @param[in] m Size of board.
 *
 * @return CONTINUE if all arguments were correct.
 * @return INPUT_ERROR if there was an input error.
 */
int init(int n, int k, int p, int x1, int y1, int x2, int y2, TreeNode **tree,
         int *nMy, int *kMy, int *playerNumber, char **board, int *m);

/**
 * Makes a move.
 *
 * @param[in] x1 Column number before a move.
 * @param[in] y1 Row number before a move.
 * @param[in] x2 Column number after a move.
 * @param[in] y2 Row number before a move.
 * @param[in] n Size of a board.
 * @param[in] players1Turn Information if it is first player who moves a Piece.
 * @param[in,out] tree Tree containing nodes with all existing pieces.
 * @param[in] p1Initialized Information if INIT of the first player has already
 * been called.
 * @param[in] p2Initialized Information if INIT of the second player has
 * already been called.
 * @param[in,out] board Array representing top left part of a board.
 * @param[in] m Size of board.
 * @param[in] turns Number of remaining turns.
 *
 * @return CONTINUE if all arguments were correct.
 * @return P1_WIN if second player's king is beaten.
 * @return P2_WIN if first player's king is beaten.
 * @return DRAW if both kings are beaten.
 * @return INPUT_ERROR if there was an input error.
 */
int move(int x1, int y1, int x2, int y2, int n, bool myTurn, TreeNode **tree,
         char **board, int m, int turns, int myNumber);

/**
 * @brief Produces a knight.
 *
 * Knight can be produced only by a peasant who wasn't moved for 2 turns.
 * Target field cannot be occupied.
 *
 * @param[in] x1 Column number of a peasant.
 * @param[in] y1 Row number of a peasant.
 * @param[in] x2 Column number of a free field where peasant is to produce
 * knight to.
 * @param[in] y2 Row number of a free field where peasant is to produce
 * knight to.
 * @param[in,out] tree Tree containing nodes with all existing pieces.
 * @param[in] p1Initialized Information if INIT of the first player has already
 * been called.
 * @param[in] p2Initialized Information if INIT of the second player has
 * already been called.
 * @param[in,out] board Array representing top left part of a board.
 * @param[in] m Size of board.
 * @param[in] turns Number of remaining turns.
 *
 * @return CONTINUE if all arguments were correct.
 * @return INPUT_ERROR if there was an input error.
 */
int produceKnight(int x1, int y1, int x2, int y2, int n, bool myTurn,
                  int myNumber, TreeNode **tree, char **board, int m,
                  int turns);

/**
 * @brief Produces a peasant.
 *
 * Peasant can be produced only by a peasant who wasn't moved for 2 turns.
 * Target field cannot be occupied.
 *
 * @param[in] x1 Column number of an existing peasant.
 * @param[in] y1 Row number of an existing peasant.
 * @param[in] x2 Column number of a free field where peasant is to produce a
 * new peasant to.
 * @param[in] y2 Row number of a free field where peasant is to produce a
 * new peasant to.
 * @param[in,out] tree Tree containing nodes with all existing pieces.
 * @param[in] p1Initialized Information if INIT of the first player has already
 * been called.
 * @param[in] p2Initialized Information if INIT of the second player has
 * already been called.
 * @param[in,out] board Array representing top left part of a board.
 * @param[in] m Size of board.
 * @param[in] turns Number of remaining turns.
 *
 * @return CONTINUE if all arguments were correct.
 * @return INPUT_ERROR if there was an input error.
 */
int producePeasant(int x1, int y1, int x2, int y2, int n, bool myTurn,
                   int myNumber, TreeNode **tree, char **board, int m,
                   int turns);

/**
 * @brief Ends a turn.
 *
 * Changes current player. Checks if both players ended their turns, then
 * "refreshes" all pieces before next turn and changes and decreases
 * remaining number of turns.
 *
 * @param[in,out] turns Number of remaining turns.
 * @param[in,out] players1Turn Informs if turn being ended was player 1's.
 * @param[in,out] tree Tree containing nodes with all existing pieces.
 * @param[in] p1Initialized Information if INIT of the first player has already
 * been called.
 * @param[in] p2Initialized Information if INIT of the second player has
 * already been called.
 * @param[in] win Logical value used by AI if it beat opponent's king.
 *
 * @return Result depending on process of last turn (ex. P1_WIN, DRAW etc.)
 */
int endTurn(int *turns, bool *myTurn, int myNumber, TreeNode *tree, bool win);

/**
 * Prints (into stdout) top-left corner of the board of size m x m.
 *
 * @param[in] board Array representing top left part of a board.
 * @param[in] m Size of board.
 */
void printTopleft(char **board, int m);

/**
 * Allocates space for m by m array and fills it with dot.
 *
 * @param [in] m Size of the array.
 * @return allocated array
 */
char **allocateMemory(int m);

/**
 * Returns lesser value of two ints.
 *
 * @param[in] a First value.
 * @param[in] b Second value.
 *
 * @return lesser value.
 */
int min(int a, int b);

/**
 * Finds lesser of two distances: between x or y coordinates.
 *
 * @param[in] x1 X coordinate of first point on board.
 * @param[in] y1 Y coordinate of first point on board.
 * @param[in] x2 X coordinate of second point on board.
 * @param[in] y2 Y coordinate of second point on board.
 *
 * @return Minimum of |x1 - x2| and  |y1 - y2|.
 */
int metricMin(int x1, int y1, int x2, int y2);

/**
 * Searches tree for node containing a piece of given x and y.
 *
 * @param [in] x First coordinate of piece that is looked for.
 * @param [in] y Second coordinate of piece that is looked for.
 * @param [in] node Tree containing nodes with all pieces.
 *
 * @return found node or NULL.
 */
TreeNode *findNode(int x, int y, TreeNode *node);

/**
 * Creates piece of given parameters.
 *
 * @param [in] x First coordinate of a new piece.
 * @param [in] y Second coordinate of a new piece
 * @param [in] ch Symbol (K, C or R) of a new piece.
 * @param [in] turns Remaining number of turns to describe when piece was moved.
 * @param [in] myTurn True if current turn is player's calling this function.
 * @param [in] myNumber Number (1 or 2) of player calling this function (to
 * define symbol of new piece, ex. 'K' or 'k'.
 *
 * @return Created piece.
 */
Piece createPiece(int x, int y, char ch, int turns, bool myTurn, int myNumber);

/**
 * Recursive removal of node containing given piece from tree.
 *
 * @param [in] piece A piece that is to be removed.
 * @param [in,out] node Root of tree containing piece to remove.
 *
 * @return Modified tree (passed with **node) without removed node.
 */
TreeNode *deleteNode(Piece piece, TreeNode **node);

/**
 * Iterative insertion of a new node cointaining given piece into tree.
 *
 * @param [in] root Root of passed tree to insert new node.
 * @param [in,out] board 2D array representing top-left corner of a board. If
 * not needed (ex. using program as ai connected to external GUI), pass NULL.
 * @param [in] m Size of board (length of side). If not needed (ex. using
 * program as ai connected to external GUI), pass anything.
 * @param [in[ piece Piece to be inserted.
 *
 * @return Modified tree (passed with *root) with new piece inserted.
 */
TreeNode *insertElement(TreeNode *root, char** board, int m, Piece piece);

/**
 * Function used to return symbol of particular unit type of given player.
 *
 * @param[in] myNumber Number of player calling this function.
 * @param[in] my Logical value describing if caller wants to know his or
 * opponent's unit's symbol. (used to avoid long ugly myNumber == 1 ? 1 : 2 in
 * first parameter)
 * @param[in] symbol Character describing symbol of which unit we want to be
 * returned ('K', 'C' or 'R').
 *
 * @return Symbol (int value of character) of given player's unit.
 */
int assignSymbol(int myNumber, bool my, char symbol);

#endif /* ENGINE_H */
