/**
 * ***** CIRCUIT *****
 *
 * Creates mathematical circuit of equations - graph, which nodes are variables, constants or result of operation.
 * Then reads lines with initialisation lists of variables and prints output of set of equations calculated by graph
 * of processes.
 *
 * @date 20.01.2017
 * @author Marcin Byra
 */

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "parser.h"
#include "err.h"
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

// Creates processes which will communicate with each other as determined by graph and calculate result when variable
// values will be passed to them.
void create_processes(Graph *graph, int number_of_equations, int const* root_dsc);

// Parses lines with initialisation lists and passes values of variables to nodes-receivers of variables.
void parse_lines(Graph* graph, int const* root_dsc, const int n, const int k, const int v);

// Frees allocated memory.
void clean_up_circuit(Graph **graph, int *root_dsc);

int main()
{
    int n, k, v;
    read_numbers(&n, &k, &v);

    // Creating local graph representing the set of definitions:
    Graph* graph = parse_equations(k, v);

    // For communication between this process and root of graph (usually x[0]) to receive ultmate result):
    int root_dsc[2];
    pipe(root_dsc);

    create_processes(graph, n - k, root_dsc);
    parse_lines(graph, root_dsc, n, k, v);

    clean_up_circuit(&graph, root_dsc);
    return 0;
}

// Returns number of nodes in Graph graph who have counterpart process-nodes.
int how_many_existing_nodes(Graph *graph)
{
    int count = 0; // = number of existing nodes seen so far
    while (count < graph->V && graph->nodes[count].what != G_NOT_EXIST)
    {
        count++;
    }

    return count;
}

// Parses initialization list. All variables values are initialized to -1 by default. If variable value appears in
// initialization list, its value is saved in array.
void parse_initialization_list(int *initialization_list_values, const int v, const int i)
{
    int ch;
    int line_no;
    int arg;
    int val;
    for (int j = 0; j < v; ++j) {
        initialization_list_values[j] = -1;
    }

    // / Description of task suggests line no is always valid, but the rest of line can be invalid
    assert(scanf("%d", &line_no) != EOF);
    
    if (getchar() == '\n') {
        return;
    }
    while ( (ch = getchar()) != '\n') {
        switch(ch){
            case ' ':
                break;
            case 'x':
                if (getchar() == '\n') {
                    return;
                }
                scanf("%d", &arg);
                if (arg == '\n') {
                    return;
                }
                if (getchar() == '\n') {
                    return;
                }
                if (getchar() == '\n') {
                    return;
                }
                scanf("%d", &val);
                if (arg == '\n') {
                    return;
                }
                if (arg >= 0 && arg < v && val >= -5000 && val <= 5000 && line_no == i) {
                    initialization_list_values[arg] = val;
                }
                else {
//                    fatal("Wrong argument or value in initialization list\n");
                }
            default:
                assert(true);
        }
    }

}

// Returns identifier of first node-variable receiver in graph.
int first_receiver_no(Graph *graph)
{
    int count = 0;
    while(graph->nodes[count].what != G_VAR_RECEIVER) {
        count++;
    }
    return count;
}

// Functions extracted from child(), responsible for performing proper actions by different types of nodes:

void receiver(Graph* graph, const int lines, const int i)
{
    char msg[30];
    int edge_nr;
    int count = 0;
    while(count++ < lines) {
        // Read message from main process:
        read(graph->pipes_to_variables[graph->nodes[i].variable_no][0], msg, sizeof(msg));
        // And send it to all variables holding the my variable
        struct Adj_List_Node* head = graph->arrayOut[i].head;
        while(head != NULL) {
            edge_nr = number_of_edge(graph, i, head->no);
            assert(edge_nr !=  -1);
            write(graph->pipes[edge_nr][1], msg, sizeof(msg));
            head = head->next;
        }
    }
}

void var(Graph* graph, const int lines, const int i, int const* root_dsc)
{
    char msg[30], msg2[30];
    int edge_nr;
    int count = 0;

    while (count++ < lines) {
        edge_nr = number_of_edge(graph, graph->arrayIn[i].head->no, i); // edge from receiver
        assert(edge_nr !=  -1);
        read(graph->pipes[edge_nr][0], msg, sizeof(msg));
        if(msg[0] == '0') { // I have no value, I must read calculated value from my son

            if (graph->arrayIn[i].head->next == NULL) { // I don't have son to read calculated value from
                if (graph->arrayOut[i].head == NULL) { // I don't have any father so I am root.
                    write(root_dsc[1], msg, sizeof(msg));
                }
                else {
                    edge_nr = number_of_edge(graph, i, graph->arrayOut[i].head->no); // edge from me to my father
                    assert(edge_nr !=  -1);
                    write(graph->pipes[edge_nr][1], msg, sizeof(msg)); // just pass message with failure info to father
                }
            }
            else { // I have a son
                edge_nr = number_of_edge(graph, graph->arrayIn[i].head->next->no, i);
                assert(edge_nr !=  -1);
                read(graph->pipes[edge_nr][0], msg, sizeof(msg));
                if (graph->arrayOut[i].head == NULL) { // I don't have any father so I am root.
                    write(root_dsc[1], msg, sizeof(msg));
                }
                else {
                    edge_nr = number_of_edge(graph, i, graph->arrayOut[i].head->no); // edge from me to my father
                    assert(edge_nr !=  -1);
                    write(graph->pipes[edge_nr][1], msg, sizeof(msg)); // just pass message to father no matter if this a failure or not
                }
            }
        }
        else { // I got the value, I need to read values from son to unblock him but not look at it and send value from my receiver to my father
            if (graph->arrayIn[i].head->next != NULL) { // I don't have son to read calculated value from
                edge_nr = number_of_edge(graph, graph->arrayIn[i].head->next->no, i);
                assert(edge_nr != -1);
                read(graph->pipes[edge_nr][0], msg2, sizeof(msg));
            }
            // I just pass the msg I got from receiver to my father:
            if (graph->arrayOut[i].head == NULL) { // I don't have any father so I am root.
                write(root_dsc[1], msg, sizeof(msg));
            }
            else {
                edge_nr = number_of_edge(graph, i, graph->arrayOut[i].head->no); // edge from me to my father
                assert(edge_nr !=  -1);
                write(graph->pipes[edge_nr][1], msg, sizeof(msg)); // just pass message with failure info to father
            }
        }
    }
}

void minus(Graph* graph, const int lines, const int i)
{
    char msg[30];
    int edge_nr;
    int count = 0;
    while (count++ < lines) {
        edge_nr = number_of_edge(graph, graph->arrayIn[i].head->no, i); // edge from my son to me
        assert(edge_nr !=  -1);
        read(graph->pipes[edge_nr][0], msg, sizeof(msg));
        edge_nr = number_of_edge(graph, i, graph->arrayOut[i].head->no); // edge from me to my father
        assert(edge_nr !=  -1);
        if (msg[0] == '0') { // there was a failure already
            write(graph->pipes[edge_nr][1], msg, sizeof(msg)); // just pass message with failure info to father
        }
        else {
            sprintf(msg + 1, "%ld", -atol(msg+1));
            write(graph->pipes[edge_nr][1], msg, sizeof(msg)); // just pass message with failure info to father
        }
    }
}

void binary_operator(Graph *graph, const int lines, const int i)
{
    long encoded_msg;
    char msg[30], msg2[30];
    int edge_nr;
    int count = 0;

    while (count++ < lines) {
        edge_nr = number_of_edge(graph, graph->arrayIn[i].head->no, i); // edge from my son to me
        assert(edge_nr !=  -1);
        read(graph->pipes[edge_nr][0], msg, sizeof(msg));
        edge_nr = number_of_edge(graph, graph->arrayIn[i].head->next->no, i); // edge from my second son to me
        assert(edge_nr !=  -1);
        if (msg[0] == '0') { // there was a failure already
            read(graph->pipes[edge_nr][0], msg2, sizeof(msg));// but I read from second son to unblock descriptor
            edge_nr = number_of_edge(graph, i, graph->arrayOut[i].head->no); // edge from me to my father
            assert(edge_nr !=  -1);
            write(graph->pipes[edge_nr][1], msg, sizeof(msg)); // just pass message with failure info to father
        }
        else {
            encoded_msg = atol(msg+1);
            read(graph->pipes[edge_nr][0], msg, sizeof(msg));// read value sent by second son
            edge_nr = number_of_edge(graph, i, graph->arrayOut[i].head->no); // edge from me to my father
            assert(edge_nr !=  -1);
            if (msg[0] == '0') { // there was a failure already
                write(graph->pipes[edge_nr][1], msg, sizeof(msg)); // just pass message with failure info to father
            }
            else  {
                // add values and save result into msg:
                sprintf(msg + 1, "%ld", graph->nodes[i].what == G_ADD ? encoded_msg + atol(msg+1) : encoded_msg * atol(msg+1));
                write(graph->pipes[edge_nr][1], msg, sizeof(msg)); // send msg to father
            }
        }
    }
}

void constant(Graph *graph, const int lines, const int i)
{
    char msg[30];
    int edge_nr;
    int count = 0;

    // I have only one parent; send him my value as many times as he needs, that is number of lines
    msg[0] = '1';
    sprintf(msg + 1, "%d", graph->nodes[i].const_value);
    while (count++ < lines) {
        edge_nr = number_of_edge(graph, i, graph->arrayOut[i].head->no);
        assert(edge_nr !=  -1);
        write(graph->pipes[edge_nr][1], msg, sizeof(msg));
    }
}

// Function deciding what type will process be.
void child(Graph *graph, const int i, const int lines, int const* root_dsc)
{
    switch(graph->nodes[i].what) {
        case G_VAR_RECEIVER:
            receiver(graph, lines, i);
            break;
        case G_VAR:
            var(graph, lines, i, root_dsc);
            break;
        case G_MINUS:
            minus(graph, lines, i);
            break;
        case G_ADD:
        case G_MULTIPLY:
            binary_operator(graph, lines, i);
            break;
        case G_CONST:
            constant(graph, lines, i);
            break;
        default:
            assert(true);
    }
}

void create_processes(Graph *graph, int number_of_equations, int const* root_dsc)
{
    int number_of_nodes = how_many_existing_nodes(graph);
    for (int i = 0; i < number_of_nodes; ++i) { // for every used (existing) node
        switch (fork()) {
            case -1:
                syserr("Error in fork\n");
            case 0:
                child(graph, i, number_of_equations, root_dsc);
                remove_graph(&graph);
                exit(1);
            default:
                break;
        }
    }
}

void parse_lines(Graph* graph, int const* root_dsc, const int n, const int k, const int v)
{
    int* initialisation_list_values = create_int_array(v); // allocate array for marking parsed values each line
    int receiver = first_receiver_no(graph); // first node that is used for receiving value of variable i from this process
    int current_variable;
    int ile;
    char msg[30];

    for (int i = k + 1; i <= n; ++i) {
        parse_initialization_list(initialisation_list_values, v, i);

        for (int j = 0; graph->nodes[receiver + j].what == G_VAR_RECEIVER; ++j) {

            current_variable = graph->nodes[receiver + j].variable_no;
            if(initialisation_list_values[j] == -1) {
                msg[0] = '0';
                write(graph->pipes_to_variables[current_variable][1], msg, sizeof(msg));
            }
            else {
                msg[0] = '1';
                ile = sprintf(msg + 1, "%d", initialisation_list_values[j]);
                msg[1 + ile] = '\0';
                write(graph->pipes_to_variables[current_variable][1], msg, sizeof(msg));
            }
        }
        read(root_dsc[0], msg, sizeof(msg));
        if(msg[0] == '0') {
            printf("%d F\n", i);
        }
        else {
            printf("%d P %ld\n", i, atol(msg+1));
        }
    }

    free(initialisation_list_values);
}

void clean_up_circuit(Graph **graph, int *root_dsc)
{
    for (int i = 0; i < how_many_existing_nodes(*graph); ++i) {
        wait(0);
    }

    close(root_dsc[0]);
    close(root_dsc[2]);
    remove_graph(graph);
}