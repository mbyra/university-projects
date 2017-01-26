/**
 * Implementation of parser functions.
 *
 * @date 21.01.2017
 * @author Marcin Byra
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "parser.h"
#include "stack.h"
#include "err.h"

void read_numbers(int *n, int *k, int *v)
{
    if (scanf ("%d %d %d", n, k, v) == EOF) {
        syserr ("Error in read_numbers\n");
        exit (1);
    }
    getchar(); // not parsed '\n'
}

// Reads one integer from input and returns it. Prints error and exit(1) if an error occurs.
int read_int()
{
    int line_no;
    if (scanf("%d", &line_no) == EOF) {
        syserr("Error in parse_equations: scanf line_no\n");
        exit (1);
    }

    return line_no;
}

// Parses lhs of equation. Marks defined variable as appeared on rhs in variables struct.
// If variable has been defined already or didn't appear on rhs anywhere and it's not first equation
// returns -1; otherwise returns number of defined variable.
int parse_lhs(Variables* variables, const int k)
{
    int ch;
    int defined_variable;
    ch = getchar();
    assert (ch == ' '); // first space after number of line
    ch = getchar();
    assert (ch == 'x');
    ch = getchar();
    assert (ch == '[');
    defined_variable = read_int();
    if (variables->x[defined_variable].lhs == true || (k > 0 && variables->x[defined_variable].rhs == false)) {
        // this variable was defined already or its not a first equation and defined variable doesn't appeared on rhs
        return -1;
    }
    variables->x[defined_variable].lhs = true; // mark this variable as defined
    ch = getchar();
    assert (ch == ']');

    // Parse middle:
    ch = getchar();
    assert (ch == ' ');
    ch = getchar();
    assert (ch == '=');
    ch = getchar();
    assert (ch == ' ');
    return defined_variable;
}

// Extracted part of switch responsible for parsing variable (after character 'x' is read).
// Pushes appropriate Thing on stack s and marks variable as appeared on rhs in variables struct.
// If detects a cycle (used variable has been defined already), returns -1. Otherwise returns 0.
int parse_equations_parse_rhs_variable(Stack *s, Variables *variables)
{
    int ch = getchar();
    assert (ch == '[');
    ch = getchar();
    assert(isdigit(ch));
    ungetc(ch, stdin);
    ch = read_int();
    if (variables->x[ch].lhs == true) {
        // variable used to define current defined_variable in equation has been already used -> CYCLE.
        return -1;
    }
    push(s, create_Thing(S_VAR, ch));
    variables->x[ch].rhs = true; // variable ch appeared on rhs; maybe not first time
    ch = getchar();
    assert(ch == ']');
    return 0;
}

// Changes enum types from stack types to graph types. For example: if argument is S_ADD, returns G_ADD, etc.
What_On_Graph change_to_graph_type(WhatOnStack what)
{
    switch (what) {
        case S_ADD:
            return G_ADD;
        case S_MINUS:
            return G_MINUS;
        case S_MULTIPLY:
            return G_MULTIPLY;
        case S_VAR:
            return G_VAR;
        case S_CONST:
            return G_CONST;
        default:
            assert (what != S_NODE && what != S_OPENING);
    }
}

// There can be alone variable or const - all lhs is var or const, no parenthesis.
// It's different case than parsing expression inside parenthesis, that's why there is special function.
void parse_equations_parse_alone_const_or_var(Stack *s, int* node_no, int *node_upper_no, Graph** graph)
{
    Thing thing = pop(s);
    assert(thing.what == S_CONST || thing.what == S_VAR);
    (*node_no)++;
    *node_upper_no = *node_no;
    (*graph)->nodes[*node_upper_no] = create_node(thing.what == S_CONST ? G_CONST : G_VAR, thing.value);
}

// The name speaks for itself.
// If detects a cycle (used variable has been defined already), returns -1. Otherwise returns 0.
int parse_equation_parse_expression_inside_parenthesis(Stack *s, int *node_no, int *node_first_no, int *node_second_no,
                                                        int *node_upper_no, Graph **graph, Variables* variables)
{

    // Rewinding to closest opening bracket. Certainly a sequence: sth, operator, sth, ( will appear
    // or sth, operator, ( if operator is unary -.
    Thing thing = pop(s);
    assert(thing.what == S_VAR || thing.what == S_CONST || thing.what == S_NODE);

    // if thing is already a node: don't create new identifier
    *node_no = (thing.what == S_NODE) ? *node_no : *node_no + 1;
    // if thing is already a node: assign node's identifier instead of current node_id to node_first_no
    *node_first_no = (thing.what == S_NODE) ? thing.value : *node_no;
    if (thing.what != S_NODE) { // if thing is a node (parsed parenthesis) there is no need to create new node.
        (*graph)->nodes[*node_first_no] = create_node(change_to_graph_type(thing.what), thing.value);
    }
    if (thing.what == S_VAR) { // if I add node on graph to variable's list of nodes where it appears
        if (variables->x[thing.value].lhs == true) {
            // variable used to define current defined_variable in equation has been already used -> CYCLE.
            return -1;
        }
        add_new_node(&(*variables).x[thing.value].nodes, *node_first_no);
    }

    // the operator:
    thing = pop(s);
    assert(thing.what == S_ADD || thing.what == S_MULTIPLY || thing.what == S_MINUS);

    (*node_no)++;
    *node_upper_no = *node_no;
    (*graph)->nodes[*node_upper_no] = create_node(change_to_graph_type(thing.what), -1);
    add_edge(*graph, *node_first_no, *node_upper_no);

    if (thing.what != S_MINUS) {
        // another operand:
        thing = pop(s);
        assert(thing.what == S_VAR || thing.what == S_CONST || thing.what == S_NODE);
        // if thing is already a node: don't create new identifier
        *node_no = (thing.what == S_NODE) ? *node_no : *node_no + 1;
        // if thing is already a node: assign node's identifier instead of current node_id to node_second_no
        *node_second_no = (thing.what == S_NODE) ? thing.value : *node_no;
        if (thing.what != S_NODE) { // if thing is a node (parsed parenthesis) there is no need to create new node.
            (*graph)->nodes[*node_second_no] = create_node(change_to_graph_type(thing.what), thing.value);
        }
        if (thing.what == S_VAR) { // if I add node on graph to variable's list of nodes where it appears
            if (variables->x[thing.value].lhs == true) {
                // variable used to define current defined_variable in equation has been already used -> CYCLE.
                return -1;
            }
            add_new_node(&(*variables).x[thing.value].nodes, *node_second_no);
        }
        add_edge(*graph, *node_second_no, *node_upper_no);
    }

    // the opening bracket:
    thing = pop(s);
    assert(thing.what == S_OPENING);

    push(s, create_Thing(S_NODE, *node_upper_no));
    return 0;
}

// Utility function that reallocates stack.
// If error == true, what means function is called after parsing invalid equation (before exit), also reallocates graph,
// stack, prints error message and exits program.
void clean_up(Graph** graph, Stack* stack, const int line_no, const bool error)
{
    remove_stack(stack);
    if(error) {
        printf("%d F\n", line_no);
        remove_graph(graph);
        exit(1);
    }
}

// After parsing all equations, creates v nodes for variables which will receive values for each variable and send it
// to all nodes holding this variable so that the main process will send values only to one node per variable.
void parse_equations_add_variable_receiver_nodes(Graph *graph, const int number_of_variables) {
    // find first unused node on graph:
    int number = 0;
    while (graph->nodes[number].what != G_NOT_EXIST) {
        number++;
    }

    struct Int_List_Node* head;
    for (int i = 0; i < number_of_variables; ++i) {
        // create receiver node for variable i on first unused node on graph
        graph->nodes[number] = create_node(G_VAR_RECEIVER, i);

        // Add edges for every node representing variable i:
        head = graph->variables->x[i].nodes.head;
        while(head != NULL) {
            add_edge(graph, number, head->node_no);
            head = head->next;
        }
        number++;
    }
}

Graph* parse_equations(const int k, const int v)
{
    Graph *graph = create_graph(5 * v); // v is maximal number of variables. In
    graph->variables = create_variables(v);
    Stack s = create_stack(5 * v); // stack to parse equation
    int node_no = -1; // previously used new node identifier

    // used to create nodes based on Thing thing and Thing operation and then to compose a graph
    int node_first_no = -1, node_second_no = -1, node_upper_no = -1;
    Thing thing, operation; // variables on which stack will pop
    int ch = -1;

    for (int i = 0; i < k; ++i) {
        const int line_no = read_int();
        const int defined_variable = parse_lhs(graph->variables, i);
        if (defined_variable == -1) {
            clean_up(&graph, &s, line_no, true);
        }

        // Parse rhs:
        while ((ch = getchar()) != '\n') {

            switch (ch) {
                case ' ':
                    break;
                case '(':
                    push(&s, create_Thing(S_OPENING, -1));
                    break;
                case '*':
                    push(&s, create_Thing(S_MULTIPLY, -1));
                    break;
                case '+':
                    push(&s, create_Thing(S_ADD, -1));
                    break;
                case '-':
                    push(&s, create_Thing(S_MINUS, -1));
                    break;
                case 'x':
                    if (is_empty(&s)) {
                        if (parse_equations_parse_rhs_variable(&s, graph->variables) == -1) {
                            clean_up(&graph, &s, line_no, true);
                        }
                        parse_equations_parse_alone_const_or_var(&s, &node_no, &node_upper_no, &graph);
                    }
                    else {
                        if (parse_equations_parse_rhs_variable(&s, graph->variables) == -1) {
                            clean_up(&graph, &s, line_no, true);
                        }
                    }
                    break;
                case ')':
                    if (parse_equation_parse_expression_inside_parenthesis(&s, &node_no, &node_first_no, &node_second_no,
                                                                           &node_upper_no, &graph, graph->variables) == -1) {
                        clean_up(&graph, &s, line_no, true);
                    }
                    break;
                default:
                    assert(isdigit(ch)); // i.e. I have a constant
                    ungetc(ch, stdin);
                    scanf("%d", &ch);
                    if(is_empty(&s)){
                        push(&s, create_Thing(S_CONST, ch));
                        parse_equations_parse_alone_const_or_var(&s, &node_no, &node_upper_no, &graph);

                    }
                    else {
                        push(&s, create_Thing(S_CONST, ch));
                    }
                    break;
            }
        }

        if (!is_empty(&s)) {
            // only one possible thing on stack - node being result of most outer operation in parenthesis
            assert(s.where == 0);
            assert( (thing = pop(&s)).what == S_NODE );
        }

        if (i == 0) {
            // First equation, I just create new node on graph holding result computing x[defined_variable] and being variable itself.
            node_no++;
            graph->nodes[node_no] = create_node(G_VAR, defined_variable);
            add_new_node(&graph->variables->x[defined_variable].nodes, node_no);
        }

        // I have to create out-edge to pass my result (my = current node_upper_no) to all nodes holding defined_variable.
        struct Int_List_Node* head = graph->variables->x[defined_variable].nodes.head;
        while (head != NULL) {
            add_edge(graph, node_upper_no, head->node_no);
            head = head->next;
        }

        printf("%d P\n", line_no);
    }


    // After all, create additional node on graph for every variable, which will read values from main process and send
    // it to every node holding this variable so that main process has only to send value to one node per valued variable
    parse_equations_add_variable_receiver_nodes(graph, v);

    graph->number_of_edges = how_many_edges(graph);
    graph->pipes = create_pipe_array(graph->number_of_edges);
    clean_up(&graph, &s, 0, false);
    return graph;
}
