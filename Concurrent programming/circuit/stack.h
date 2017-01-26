/**
 * Header file for stack containing Thing, used to parse equations.
 *
 * @date 22.01.2017
 * @author Marcin Byra
 */

#ifndef CIRCUIT_STACK_H
#define CIRCUIT_STACK_H

#include <stdbool.h>

typedef enum {S_VAR, S_CONST, S_ADD, S_MULTIPLY, S_MINUS, S_OPENING, S_NODE} WhatOnStack;

typedef struct {
    WhatOnStack what;
    int value; // S_VAR - number of variable, S_CONST- value of constant, S_NODE - number of node, otherwise -1
} Thing;

typedef struct {
    int where;
    size_t STACK_SIZE;
    Thing *array;
} Stack;

// Returns Stack of given size.
Stack create_stack(const size_t size);

// Removes stack s.
void remove_stack(Stack* s);

// Returns true only if stack is empty.
bool is_empty(Stack const* s);

// Returns true only if stack is full.
bool is_full(Stack const* s);

// Return element which is on top of the stack without removing it.
Thing top(Stack const* s);

// Returns the element which is on top of the stack and removes it.
Thing pop(Stack* s);

// Pushes t on the top of stack.
void push (Stack* s, const Thing t);

// Creates Thing based on type and value and returns it.
Thing create_Thing(WhatOnStack type, int value);

#endif //CIRCUIT_STACK_H
