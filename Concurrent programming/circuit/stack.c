/**
 * Implementation of stack
 *
 * @date 22.01.2017
 * @author Marcin Byra
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

bool is_empty(Stack const* s)
{
    return s->where == -1;
}

bool is_full(Stack const* s)
{
    return s->where == s->STACK_SIZE - 1;
}

Thing top(Stack const* s) {
    if (is_empty(s)) {
        printf("top: stack is empty!\n");
        exit (1);
    }
    return s->array[s->where];
}

Thing pop(Stack* s) {
    if (is_empty(s)) {
        printf("pop: stack is empty!\n");
        exit (1);
    }

    return s->array[s->where--];
}

void push(Stack* s, Thing t) {
    s->array[++(s->where)] = t;
}

Thing create_Thing(WhatOnStack type, int value) {
    Thing result;
    result.what = type;
    result.value = value;
    if (result.what != S_VAR && result.what != S_CONST && result.what != S_NODE) {
        result.value = -1;
    }

    return result;
}

Stack create_stack(const size_t size) {
    Stack result;
    result.STACK_SIZE = size;
    result.where = -1;
    result.array = (Thing*) calloc(size, sizeof (Thing));

    return result;
}

void remove_stack(Stack* s) {
    free(s->array);
}





