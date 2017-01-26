/**
 * Declarations of functions used to read data from input to dynamically allocated arrays and to print and free these
 * arrays.
 *
 * @date 21.01.2017
 * @author Marcin Byra
 */

#ifndef GRAPH2_PARSER_H
#define GRAPH2_PARSER_H

#include "graph.h"

/**
 * Parses row containing:
 * n - number of rows ofb input (not counting this one,
 * k - number of equations describing the circuit,
 * v - maximal number of variables in circuit.
 * */
void read_numbers(int *n, int *k, int *v);

/**
 * Parses k rows containing equations describing graph using v variables.
 * Returns pointer to graph containing of two lists of adjacency (in and out) of each node represented by identifiers
 * and array mapping identifier to Node.
 * Since v is maximal number of variables, number of nodes may be much higher
 * If equation is valid, prints "i P", where i is a number of input row.
 * Otherwise prints "i F" and exits.
 */
Graph* parse_equations(const int k, const int v);

#endif //GRAPH2_PARSER_H
