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

//typedef std::vector<std::vector<int>> Graph;

// Variables used to create, hold and access the graph:
std::vector<std::vector<int>> graph; // vertex: list_of_neighbours
int identifier = -1; // previously used identifier
std::unordered_map<int, int> index_map; // to map vertices to successive identifiers
std::vector<int> index_map_reverse; // for const-time access to original vertex number
std::vector<int> original_order;

// Variables used to calculate betweenness centrality of each node:
std::vector<double> BC; // results
int index = 0; // next index of vertex in graph to calculate
std::mutex index_mutex; // index protecting access to index
std::mutex* BC_mutex; // pointer to array of mutexes protecting access to every index of BC array


// Prints graph:
// node_no: <list of neighbors>
void print_graph ()
{
    for ( auto i = 0; i < graph.size(); ++i ) {
        std::cout << "Original:" << std::endl;
        std::cout << i << ": ";
        for ( auto neighbor : graph[i] ) {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl << "Mapped:  " << std::endl;
        std::cout << index_map_reverse[i] << ": ";
        for ( auto neighbor : graph[i] ) {
            std::cout << index_map_reverse[neighbor] << " ";
        }
        std::cout << std::endl << std::endl;
    }
}

// Reads input from given filename.
// Returns representation of graph (vector of neighbors of subsequent nodes);
void parse_input (std::ifstream& input_file)
{
    if ( !input_file.is_open() ) {
        std::cout << "Could not open input file" << std::endl;
        std::exit(1);
    }

    int out, in;
    int last = -1;

    while ( input_file >> out ) {

        input_file >> in;

        if (last < out) {
            original_order.push_back(out);
            last = out;
        }

        auto find = index_map.find( out );
        if ( find == index_map.end() ) { // this node hasn't appeared yet
            index_map[out] = ++identifier; // map out to the identifier
            // assign to identifier:
            index_map_reverse.push_back( out ); // ...original number
            graph.push_back( std::vector<int>() ); // ...empty list of neighbors
        } // else: vertex already exists on index_maps and on graph

        find = index_map.find( in );
        if ( find == index_map.end() ) { // // this node hasn't appeared yet
            index_map[in] = ++identifier; // map out to the identifier
            // assign to identifier:
            index_map_reverse.push_back(in); // ...original number
            graph.push_back(std::vector<int>()); // empty list of neighbors
        } // else: vertex already exists on index_maps and on graph

        // Adding in to neighbor list of out:
        graph[ index_map[out] ].push_back( index_map[in] );

    }

}

int next_index()
{
    index_mutex.lock();
    int next_index;
    if (index < graph.size()) {
        next_index = index;
        index++;
    }
    else {
        next_index = -1;
    };
    index_mutex.unlock();
    return next_index;
}

// Passing copy of BC (list of nodes with betweenness = 0) to avoid filling it again
void brandes_single_thread( )
{

    std::vector<std::vector<int>> P(graph.size());
    std::vector<int> sigma(graph.size());
    std::vector<int> d(graph.size());
    std::vector<double> delta(graph.size());

    int s;
    while ( (s = next_index()) != -1 ) {

        std::stack<int> S;

        for ( auto w = 0; w < graph.size(); w++ ) {
            P[ w ] = std::vector<int>();
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
            Q.pop( );
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
                delta[ v ] += ( double(sigma[ v ]) / sigma[ w ] ) * ( 1 + delta[ w ] );
            }

            if ( w != s ) {
                BC_mutex[ w ].lock();
                BC[ w ] += delta[ w ];
                BC_mutex[ w ].unlock();
            }
        }
    }

}

// Brandes algorithm finding betweenness of each node of graph.
// Returns vector of pairs: <node_no, betweenness>
int main ( int argc, char* argv[] )
{
    if ( argc != 4 ) {
        std::cout << "Usage: " << argv[0] << " <number of threads> <input file> <output file>" << std::endl;
        return 1;
    }

    const int number_of_threads = atoi( argv[1] );
    std::ifstream input_file ( argv[2] );
    std::ofstream output_file ( argv[3] );

    parse_input( input_file );

    if ( !output_file.is_open() ) {
        std::cout << "Could not open output file" << std::endl;
        std::exit(1);
    }

    size_t size = graph.size();
    BC = std::vector<double>(size, 0); // results

    BC_mutex = new std::mutex[size]; // allocate array of mutexes (must be done in runtime)

    // Run threads calculating BC:
    std::vector<std::thread> threads;
    for ( auto i = 0; i < number_of_threads; i++ ) {
        threads.push_back( std::thread { brandes_single_thread } );
    }

    // Wait for the threads to complete:
    for ( auto i = 0; i < number_of_threads; ++i ) {
        threads[i].join();
    }

    // Print output to file:
    for (auto i : original_order) {
        if ( !graph[ index_map[i] ].empty() ) {
            output_file << i << " " << BC[ index_map[i] ] << std::endl;
        }
    }

    delete[] BC_mutex;
}
