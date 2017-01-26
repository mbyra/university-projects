/**
 * Implementation of graph.
 *
 * @date 22.01.2017
 * @author Marcin Byra
 */

#include "graph.h"
#include "err.h"
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <assert.h>

// Adds new node with node_no = no to List l:
void add_new_node(List* l, const int no)
{
    struct Int_List_Node* new_node = (struct Int_List_Node*) malloc (sizeof (struct Int_List_Node));
    new_node->node_no = no;
    new_node->next = l->head;
    l->head = new_node;

}

// Removes all nodes of List l;
void remove_list(List l)
{
    struct Int_List_Node* current = l.head;
    struct Int_List_Node* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    l.head = NULL;
}

// Creates Variables consisting of v Variable_Nodes, initially with lhs = rhs = false
// and empty list of nodes holding this variable/
Variables* create_variables(const int v)
{
    Variables* result = (Variables*) malloc(sizeof (Variables));
    result->v = v;
    result->x = (Variables_Node*) malloc(v * sizeof(Variables_Node));
    for (int i = 0; i < v; ++i) {
        result->x[i].lhs = result->x[i].rhs = false;
        result->x[i].nodes.head = NULL;
    }

    return result;
}

// Removes all array x of Variables var and all lists of nodes of every variable in x.
void remove_variables(Variables* var)
{
    for (int i = 0; i < var->v; ++i) {
        remove_list(var->x[i].nodes);
    }
    free(var->x);
    free(var);
}

// Print all nodes in List list.
void print_list(List *list)
{
    struct Int_List_Node* head = list->head;
    while (head != NULL) {
        printf("%d ", head->node_no);
        head = head->next;
    }
    printf("\n");
}

// Helper function for is_acyclic to do recursive search (DFS) on graph
bool is_cyclic_helper_recursive(Graph *graph, Variables *variables, bool** visited, bool** rec_stack, int v)
{
    if ((*visited)[v] == false) {
        // Mark the current node as visited and part of recursion stack
        (*visited)[v] = true;
        (*rec_stack)[v] = true;

        // Recur for all the vertices adjacent to this vertex
        struct Adj_List_Node* head = graph->arrayIn[v].head;
        while(head != NULL) {
//            if (!(*visited)[head->no] && is_cyclic_helper_recursive(graph, variables, visited, rec_stack, head->no)) {
//                return true;
//            }
//            else if ((*rec_stack)[head->no] == true) {
//                return true;
//            }
            if(graph->nodes[head->no].what == G_VAR) {
                // if this node represents variable...
                int variable_no = graph->nodes[head->no].variable_no;
                if(variables->x[variable_no].lhs == true) {
                    // ...and this variable's been defined already (is on lhs list)
                    return true; // then return true cause graph has a cycle
                }
            }
            head = head->next;
        }
    }
    (*rec_stack)[v] = false; // remove the vertex from recursion stack
    return false;
}

// Returns true if graph is acyclic, false if there is a cycle.
// A cycle could've appeared only after linking definition (subgraph) to some variable node, so function performs DFS
// starting from this variable node.
bool is_cyclic(Graph *graph, Variables *variables, const int starting_node)
{
    // Mark all the vertices as not visited and not part of recursion stack
    bool *visited = (bool*) calloc(graph->V, sizeof (bool));
    bool *rec_stack = (bool*) calloc(graph->V, sizeof (bool));
    for (int i = 0; i < graph->V; ++i) {
        visited[i] = false;
        rec_stack[i] = false;
    }

    bool result = is_cyclic_helper_recursive(graph, variables, &visited, &rec_stack, starting_node);
    free(visited);
    free(rec_stack);
    return result;
}

// For every variable i in Variables pointed by var prints if i appeared on lhs and on rhs of any equation:
void print_variables(Variables const* var)
{
    printf("Variables:\n");
    for (int i = 0; i < var->v; ++i) {
        printf("%d: lhs ", i);
        if (var->x[i].lhs) {
            printf("+ rhs ");
        }
        else {
            printf("- rhs ");
        }
        if (var->x[i].rhs) {
            printf("+ ");
        }
        else {
            printf("- ");
        }
        print_list(&var->x[i].nodes);
    }
    printf("\n");
}
struct Adj_List_Node* new_adj_list_node(int dest)
{
    struct Adj_List_Node* newNode = (struct Adj_List_Node*) malloc(sizeof(struct Adj_List_Node));
    newNode->no = dest;
    newNode->next = NULL;
    return newNode;
}

void remove_Adj_List(struct Adj_List* adj_list)
{
    struct Adj_List_Node* current = adj_list->head;
    struct Adj_List_Node* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    adj_list->head = NULL;
}

// A utility function that creates a graph of V vertices
Graph* create_graph(const int v)
{
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->V = v;
    graph->number_of_edges = -1;
    graph->variables = NULL;
    graph->descriptors = create_int_array(v); // array of integers describing descriptor number of variable i:
    graph->pipes = NULL; // for every edge
    graph->pipes_to_variables = create_pipe_array(v); // from this process to all variables

    // Create an array of adjacency lists.  Size of array will be V
    graph->arrayIn = (struct Adj_List*) malloc(v * sizeof(struct Adj_List));
    graph->arrayOut = (struct Adj_List*) malloc(v * sizeof(struct Adj_List));

    // Create an array of nodes structures. Every node is defined here:
    graph->nodes = (struct Node*) malloc(v * sizeof(struct Node));

    // Initialize each adjacency list as empty by making head as NULL
    int i;
    for (i = 0; i < v; ++i) {
        graph->arrayIn[i].head = NULL;
        graph->arrayOut[i].head = NULL;
        graph->nodes[i] = create_node(G_NOT_EXIST, -1);
    }

    return graph;
}

void remove_graph(Graph** graph)
{
    Graph *to_remove = *graph;
    // remove lists of each node adjacency lists
    for (int i = 0; i < to_remove->V; ++i) {
        remove_Adj_List(&to_remove->arrayIn[i]);
        remove_Adj_List(&to_remove->arrayOut[i]);
    }

    // remove adjacency lists themselves
    free(to_remove->arrayIn);
    free(to_remove->arrayOut);

    // remove list of nodes
    free(to_remove->nodes);
    // remove variables
    if (to_remove->variables != NULL) {
        remove_variables(to_remove->variables);
    }
    remove_descriptor_array(to_remove->descriptors);
    if (to_remove->pipes != NULL) {
        remove_pipe_array(to_remove->pipes, to_remove->number_of_edges);
    }
    remove_pipe_array(to_remove->pipes_to_variables, to_remove->V);

    free(*graph);
    *graph = NULL;
}

void add_edge(struct Graph_T *graph, int src, int dest)
{
    // Add an out-edge from src to node_no.  A new node is added to the adjacency list of src.
    // The node is added at the beginning
    struct Adj_List_Node* newNode = new_adj_list_node(dest);
    newNode->next = graph->arrayOut[src].head;
    graph->arrayOut[src].head = newNode;

    // Since graph is directed and there is need to know in-edges of each node, add in-edge of node_no to src.
    newNode = new_adj_list_node(src);
    newNode->next = graph->arrayIn[dest].head;
    graph->arrayIn[dest].head = newNode;
}

void print_nodes(struct Graph_T *graph)
{
    int v;
    printf("Nodes definitions:\n");
    for (v = 0; v < graph->V; ++v) {
        if (graph->nodes[v].what != G_NOT_EXIST) {
            printf("%d: ", v);
            if (graph->nodes[v].variable_no != -1) {
                printf("x[%d]", graph->nodes[v].variable_no);
                if (graph->nodes[v].what != G_VAR) {
                    // Except of being a variable, node is also root of some subgraph (result of operator)
                    printf(", ");
                }
            }
            switch (graph->nodes[v].what) {
                case G_VAR:
                    break;
                case G_CONST:
                    printf("%d", graph->nodes[v].const_value);
                    break;
                case G_ADD:
                    printf("+");
                    break;
                case G_MULTIPLY:
                    printf("*");
                    break;
                case G_MINUS:
                    printf("-");
                    break;
                default:
                    break;
            }
            printf("\n");
        }
    }
}

// A utility function to print the adjacency list representation of graph
void print_graph(struct Graph_T *graph)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        struct Adj_List_Node* pCrawl = graph->arrayOut[v].head;
        printf("\n Vertex %d\n    out -> ", v);
        while (pCrawl)
        {
            printf("%d ", pCrawl->no);
            pCrawl = pCrawl->next;
        }

        pCrawl = graph->arrayIn[v].head;
        printf("\n    in  <- ");
        while (pCrawl)
        {
            printf("%d ", pCrawl->no);
            pCrawl = pCrawl->next;
        }
    }
    printf("\n");
    print_nodes(graph);
    print_variables(graph->variables);

}

struct Node create_node(What_On_Graph what, int value) {
    struct Node result;
    result.what = what;
    result.const_value = what == G_CONST ? value : -1;
    result.variable_no = what == G_VAR || G_VAR_RECEIVER ? value : -1;
    return result;
}

int how_many_edges(Graph *graph) {
    int count = 0;
    for (int i = 0; i < graph->V; ++i) {
        struct Adj_List_Node* head = graph->arrayOut[i].head;
        while (head != NULL) {
            count++;
            head = head->next;
        }
    }
    return count;
}


void remove_descriptor_array(int *descriptors)
{
    free(descriptors);
}

int *create_int_array(const int v)
{
    int* table = (int*) calloc(v, sizeof (int));
    for (size_t i = 0; i < v; ++i) {
        table[i] = -1;
    }
    return table;
}

int **create_pipe_array(const int v) {
    int** pipes = (int**) calloc(v, sizeof(int*));
    for (size_t i = 0; i < v; ++i) {
        pipes[i] = (int*) calloc(2, sizeof(int));
        if (pipe(pipes[i]) == -1) {
            syserr("Error in pipe\n");
        }
    }

    return pipes;
}

void remove_pipe_array(int** pipes, const int v) {
    for (size_t i = 0; i < v; ++i) {
        close(pipes[i][0]);
        close(pipes[i][1]);
        free(pipes[i]);
    }
    free(pipes);
}

int number_of_edge(Graph *graph, const int from, const int to) {
    int count = -1;
    struct Adj_List_Node* head = NULL;
    for (int i = 0; i < graph->V; ++i) {
        head = graph->arrayOut[i].head;
        while (head != NULL) {
            count++;
            if (i == from && head->no == to) {
                return count;
            }
            head = head->next;
        }
        if (i > from) {
            return -1;
        }
    }

    assert(true);
    return -1;
}
