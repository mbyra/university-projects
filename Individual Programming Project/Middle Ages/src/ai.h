/** @file
 *
 *  Interface of AI.
 *  This AI is independent of engine.c.
 *  Contains only one function, generateAction, which process all the turn
 *  (at a pinch modifies tree of pieces on board and other main variables)
 *  and returns proper result (represented by a constant).
 */

#ifndef MIDDLE_AGES_AI_H
#define MIDDLE_AGES_AI_H

#include "engine.h"

/**
 * @brief AI generating all actions during current turn.
 *
 * The function is an AI. It produces a peasant if player has only one unit of
 * this type, otherwise produces a knight. It produces new units towards
 * closest enemy piece. It also moves every knight towards closest enemy
 * piece. If it cannot move or produce in this direction, tries to do it into
 * neighbouring squares (until possible squares are further to closest enemy
 * unit than peasant). Finally, it ends turn, decreasing parameter *turns (if
 * player's number is 2), negates *myTurn and returns result.
 *
 * @param[in,out] **tree Tree containing nodes with all pieces on board.
 * @param[in,out] *turns Number of remaining turns.
 * @param[in,out] *myTurn Parameter used in middle_ages.c to define which
 * turn it is: ai's (to generate action) or other player's (to parse his
 * actions). Here only to be negated.
 * @param[in] myNum Player's number (1 or 2).
 * @param[in] n Size (of a side) of a board.
 *
 * @returns Defined constant representing the result (WIN, DRAW, CONTINUE).
 */
int generateAction(TreeNode **tree, int *turns, bool *myTurn, int myNum,
                   int n);

#endif //MIDDLE_AGES_AI_H
