#include<iostream>
#include<deque>
#include<map>
#include <cassert>

using namespace std;

#define MY_DEQUE_MAP map<unsigned long, pair<bool, deque<string>*>>

#ifndef NDEBUG
const bool debug = true;
#else
const bool debug = false;
#endif

namespace additional {

    extern MY_DEQUE_MAP & deque_map();
    extern deque<unsigned long> & free_no();
    extern unsigned long & next_no();
    extern void initStreams();

}

namespace jnp1 {

    using additional::deque_map;
    using additional::free_no;
    using additional::next_no;
    using additional::initStreams;

    extern "C" unsigned long emptystrdeque() {
        //Creates a new emptystrdeque and returns its id.
        //Insertion elements to an emptystrdeque and removing elements from an emptystrdeque are forbidden.

        if(debug) {
            initStreams();
            cerr << "emptystrdeque()" << endl;
        }

        if(free_no().empty()) {
            free_no().push_back(next_no()++);
            if(debug) {
                // next_no() == 0 means the end of available numbers
                assert(next_no() != 0);
            }
        }

        unsigned long id = free_no().front();
        free_no().pop_front();

        deque_map().emplace(id, make_pair(true, new deque<string>));

        return id;
    }
}