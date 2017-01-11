#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <list>
#include <set>
#include <stack>
#include <queue>
#include <future>
#include <sstream>
#include <unordered_map>

typedef std::vector<std::vector<int>> Graph;
typedef std::vector<double> BC_t;
typedef std::promise<BC_t> BC_promise_t;
typedef std::future<BC_t> BC_future_t;
typedef std::unordered_map<int, int> index_map_t;

// Prints graph:
// node_no: <list of neighbors>
void print_graph( const Graph& graph )
{
    for ( auto i = 0; i < graph.size(); ++i ) {
        std::cout << i << ": ";
        for ( auto neighbor : graph[i] ) {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }
}

// Reads input from given filename.
// Returns representation of graph (vector of neighbors of subsequent nodes);
void parse_input( std::ifstream& input_file, Graph& graph, index_map_t& index_map, std::list<int>& original_order )
{
    if ( !input_file.is_open() ) {
        std::cout << "Could not open input file" << std::endl;
        std::exit(1);
    }

    BC_t BC( graph.size(), 0 );

    int out, in;
    int id = -1; // id to map current node as well as index of node from data in graph
    int last = -1; // to tell if current out-node is the same as line before or not

    while ( !input_file.eof() ) {
        input_file >> out >> in;
        auto find_out = index_map.find( out );
        if ( find_out == index_map.end() ) { // this node hasn't occured yet
            id++; // new id for new node
            index_map.insert( std::make_pair( out, id ) );
            graph.push_back( std::vector<int>() );
        } // else: no need neither to add node to map nor to create new graph row

        auto find_in = index_map.find( in );
        if ( find_in == index_map.end() ) { // this node hasn't occured yet but first node (out-node) has
            id++; // new id for new node
            index_map.insert( std::make_pair( in, id ) );
            graph.push_back( std::vector<int>() ); // it's in-node but every node has to be in graph (and maybe it'll get some neighbors)
        } // else: no need neither to add node to map nor to create new graph row

        // Adding in to neighbor list of out:
        graph[ index_map[out] ].push_back( index_map[in] );

        // Adding to the list with original order of nodes to avoid sorting graph when printing to file:
        if ( last < out ) {
            last = out;
            original_order.push_back( out );
        }
    }

    // Remove repeated neighbour (caused by newline parsed) from last node:
    graph[ graph.size() - 1 ].erase( std::unique( graph[ graph.size() - 1 ].begin(), graph[ graph.size() - 1 ].end() ),
                                     graph[ graph.size() - 1 ].end() );

}

// Passing copy of BC (list of nodes with betweenness = 0) to avoid filling it again
void brandes_single_thread( const Graph& graph, BC_promise_t& BC_promise, const int a, const int b )
{
    std::vector<std::list<int>> P( graph.size() );
    std::vector<double> sigma( graph.size() );
    std::vector<double> d( graph.size() );
    std::vector<double> delta( graph.size() );
    BC_t BC ( graph.size(), 0 );

    for ( int s = a; s < b; ++s ) {
        std::stack<int> S;

        for ( Graph::size_type w = 0; w < graph.size(); ++w ) {
            P[ w ] = std::list<int>();
            sigma[ w ] = 0;
            d[ w ] = -1;
            delta[ w ] = 0;
        }

        sigma[ s ] = 1;
        d[ s ] = 0;
        std::queue<int> Q;
        Q.push( s );

        while ( !Q.empty() ) {
            int v = Q.front();
            Q.pop();
            S.push( v );

            for ( auto w : graph[ v ] ) {
                if ( d[ w ] < 0 ) {
                    Q.push( w );
                    d[ w ] = d[ v ] + 1;
                }
                if ( d[ w ] == d[ v ] + 1 ) {
                    sigma[ w ] += sigma[ v ];
                    P[ w ].push_back(v);
                }
            }
        }

        while ( !S.empty() ) {
            int w = S.top();
            S.pop();

            for ( auto v : P[ w ] ) {
                delta[ v ] += ( sigma[ v ] / sigma[ w ] ) * ( 1 + delta[ w ] );
            }

            if ( w != s ) {
                BC[ w ] += delta[ w ];
            }
        }
    }

    // Set values of promise of real BC:
    BC_promise.set_value( BC );
}

// Brandes algorithm finding betweenness of each node of graph.
// Returns vector of pairs: <node_no, betweenness>
void brandes( const Graph& graph, index_map_t& index_map, const int number_of_threads, std::ofstream& file,
              const std::list<int>& original_order )
{
    if ( !file.is_open() ) {
        std::cout << "Could not open output file" << std::endl;
        std::exit(1);
    }

    BC_t BC( graph.size(), 0 );
    std::vector<BC_promise_t> BC_promise( number_of_threads );
    std::vector<BC_future_t> BC_future( number_of_threads );


    // Round up - in case there are more threads than nodes
    int nodes_for_one_thread = ( graph.size() + number_of_threads - 1 ) / number_of_threads;
    // [a, b) is the range of indices of nodes in graph to be processed by a given thread
    int a = 0;
    int b = nodes_for_one_thread;
    // In case there is more threads than nodes; otherwise program would wait for nonexistent promise
    int number_of_threads_really_used = 0;

    std::vector<std::thread> threads;

    for ( auto i = 0; i < number_of_threads; ++i ) {

        BC_future[i] = BC_promise[i].get_future();
        threads.push_back( std::thread { [ &graph, &BC_promise, i, BC, a, b ]
                                         { brandes_single_thread( graph, BC_promise[i], a, b ); } } );
        number_of_threads_really_used++;

        if ( b > graph.size() ) {
            break; // don't start new thread if all nodes are assigned to thread
        }
        a = b;
        // last set of nodes may be shorter, that's why condition is added
        b = a + nodes_for_one_thread < graph.size() ? a + nodes_for_one_thread : graph.size();
    }

    for ( auto i = 0; i < number_of_threads_really_used; ++i ) {
        // Get future from thread:
        BC_t BC_cpy = BC_future[i].get();
        // Add values calculated by thread to real BC:
        for ( auto j = 0; j  < BC.size(); ++j ) {
            BC[j] += BC_cpy[j];
        }
        threads[i].join();
    }

    for ( auto i : original_order ) {
        if ( !graph[index_map[ i ]].empty() )
            file << i << " " << BC[index_map[ i ]] << std::endl;
    }
}

int main ( int argc, char* argv[] )
{
    if ( argc != 4 ) {
        std::cout << "Usage: " << argv[0] << " <number of threads> <input file> <output file>" << std::endl;
        return 1;
    }

    int number_of_threads = atoi( argv[1] );
    std::ifstream input_file ( argv[2] );
    std::ofstream output_file ( argv[3] );

    // i-th index doesn't have to be at i-th position, so the map is needed:
    index_map_t index_map; // real_node -> my_node
    std::list<int> original_order; // to keep original order of out-nodes (which is lost by inserting into maps)
    Graph graph;

    parse_input( input_file, graph, index_map, original_order );

    brandes( graph, index_map, number_of_threads, output_file, original_order );
}