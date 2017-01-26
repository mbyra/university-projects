/**
 * Declarations of functions used to create and manipulate Graph - the type used to keep information about relations
 * between nodes.
 *
 * @date 22.01.2017
 * @author Marcin Byra
 */

#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include <stdio.h>
#include <stdbool.h>

// List of integers.
struct Int_List_Node
{
    int node_no;
    struct Int_List_Node* next;
};

typedef struct
{
    struct Int_List_Node* head;
} List;

// Node of Variable
typedef struct
{
    bool lhs; // True if this variable appeared on left hand side of any equation.
    bool rhs; // True if this variable appeared on right hand side of any equation.
    List nodes; // List of identifiers of nodes holding this variable.
} Variables_Node;

typedef struct Variables_T
{
    int v; // maximal number of possible variables
    Variables_Node* x; // array of v variables. First element represents x[0] etc.
} Variables;

// A node can be: variable, constant, result of + or * of in-nodes, result of - of in-node.
typedef enum {G_VAR, G_CONST, G_ADD, G_MINUS, G_MULTIPLY, G_NOT_EXIST, G_VAR_RECEIVER} What_On_Graph;

// A structure of node (structure of real node, not representative):
struct Node
{
    What_On_Graph what;
    int variable_no; // number of variable represented by node (if node doesn't represent variable, then -1).
    int const_value; // value of constant if node represents constant
};

// A structure to represent an adjacency list node
struct Adj_List_Node
{
    int no; // number of node
    struct Adj_List_Node* next;
};

// A structure to represent an adjacency list
struct Adj_List
{
    struct Adj_List_Node *head;  // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists and array of definitions of each node based on its id.
// Size of array will be V (number of vertices in graph)
typedef struct Graph_T
{
    int V;
    struct Adj_List* arrayIn;
    struct Adj_List* arrayOut;
    struct Node* nodes;
    Variables* variables;
    int number_of_edges;
    int* descriptors;
    int** pipes;
    int** pipes_to_variables;
} Graph;


typedef struct Graph_T Graph;

// Adds new node with node_no = no to List l:
void add_new_node(List* l, const int no);

// Removes all nodes of List l;
void remove_list(List l);

// Creates Variables consisting of v Variable_Nodes, initially with lhs = rhs = false
// and empty list of nodes holding this variable/
Variables* create_variables(const int v);

// Removes all array x of Variables var and all lists of nodes of every variable in x.
void remove_variables(Variables* var);

// A utility function to create a new node based on what it represents, and value.
// If node represents constant, variable_no is set to -1 and const_value to value.
// If node represents variable, variable_no is set to value and const_value to -1.
struct Node create_node(What_On_Graph what, int value);

// A utility function to create a new adjacency list node
struct Adj_List_Node* new_adj_list_node(int dest);

// Removes all nodes on Adj_List adj_list
void remove_Adj_List(struct Adj_List* adj_list);

// A utility function that creates a graph for v variables
Graph* create_graph(const int v);

// A utility function that removes a graph pointed by *g
void remove_graph(Graph** graph);

// A utility function that adds edges to directed graph: out-edge from src to node_no and in-edge of node_no from src
void add_edge(Graph* graph, int src, int dest);

// Returns number of edges (outgoing) in graph
int how_many_edges(Graph *graph);

// Returns true if graph is acyclic, false if there is a cycle.
// A cycle could've appeared only after linking definition (subgraph) to some variable node, so function performs DFS
// starting from this variable node.
bool is_cyclic(Graph *graph, Variables *variables, const int starting_node);

// A utility function to print the adjacency list representation of graph
void print_graph(Graph* graph);

// For every variable i in Variables pointed by var prints if i appeared on lhs and on rhs of any equation:
void print_variables(Variables const* var);

// A utility function to print definitions of each node
void print_nodes(Graph* graph);

// Allocates array of integers of size v
int *create_int_array(const int v);

// Deallocates array of descriptors
void remove_descriptor_array(int *descriptors);

// Allocates v pairs of descriptors
int** create_pipe_array(const int v);

// Deallocates array of v pairs of descriptors
void remove_pipe_array(int** pipes, const int v);

// Return number of edge from node from to node to on graph.
// If such edge doesn't exist on graph, returns -1.
int number_of_edge(Graph* graph, const int from, const int to);

#endif //GRAPH_GRAPH_H
