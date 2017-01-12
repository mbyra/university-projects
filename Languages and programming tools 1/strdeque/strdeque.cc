#include <iostream>
#include <deque>
#include <map>
#include <cstring>
#include <cassert>
#include <algorithm>

using namespace std;

#define MY_DEQUE_MAP map<unsigned long, pair<bool, deque<string>*>>

#ifndef NDEBUG
const bool debug = true;
#else
const bool debug = false;
#endif

namespace additional {

    MY_DEQUE_MAP & deque_map() {
        //Meaning of bool in the pair: false if strdeque, true if strdequeconst.
        static MY_DEQUE_MAP deque_map;
        return deque_map;
    }

    deque<unsigned long> & free_no() {
        //This deque stores id numbers of the deleted deques from the map.
        static deque<unsigned long> free_no;
        return free_no;
    }

    unsigned long & next_no() {
        //Next number for id.
        static unsigned long next_no = 0;
        return next_no;
    }

    void initStreams() {
        ios_base::Init();
    }

}

namespace jnp1 {

    using additional::deque_map;
    using additional::free_no;
    using additional::next_no;
    using additional::initStreams;

    extern "C" {

        unsigned long strdeque_new() {
            //Creates a new strdeque and returns its id.

            string toPrint;

            if(debug) {
                initStreams();
                toPrint = "strdeque_new()\n";
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

            deque_map().emplace(id, make_pair(false, new deque<string>));

            if(debug) {

                cerr << toPrint << "strdeque_new: deque " << id << " created\n";
            }

            return id;
        }

        void strdeque_delete(unsigned long id) {
            //If the deque of the given id exists, deletes it.
            //Otherwise does nothing.

            string toPrint;

            if(debug) {
                initStreams();
                toPrint += "strdeque_delete(";
                toPrint += to_string(id);
                toPrint += ")\n";
            }

            MY_DEQUE_MAP::iterator it = deque_map().find(id);

            if (it == deque_map().end()) {
                if(debug) {
                    cerr << toPrint << "strdeque_delete: deque " << id << " does not exist" << endl;
                }
                return;
            }

            if((*it).second.first) {
                if(debug) {
                    cerr << "strdeque_delete(the Empty Deque)" << endl
                         << "strdeque_delete: attempt to remove the Empty Deque" << endl;
                }
                return;
            }

            deque_map()[id].second->clear();
            delete deque_map()[id].second;
            deque_map().erase(id);
            free_no().push_back(id);

            if(debug) {
                cerr << toPrint << "strdeque_delete: deque " << id <<" deleted" << endl;
                assert(free_no().back() == id);
            }

        }

        size_t strdeque_size(unsigned long id) {
            //If the deque of the given id exists, returns number of its elements.
            //Otherwise returns 0.

            string toPrint;

            if(debug) {
                initStreams();
                toPrint += "strdeque_size(";
                toPrint += to_string(id);
                toPrint += ")\n";
            }

            MY_DEQUE_MAP::iterator it = deque_map().find(id);

            if (it == deque_map().end()) {
                if(debug) {
                    cerr << toPrint << "strdeque_size: deque " << id << " does not exist" << endl;
                }
                return 0;
            }

            if((*it).second.first) {
                if(debug) {
                    cerr << "strdeque_size(the Empty Deque)" << endl
                         << "strdeque_size: the Empty Deque contains 0 elements" << endl;
                }
                return 0;
            }

            size_t ret = (*it).second.second->size();
            if(debug) {
                cerr << toPrint << "strdeque_size: deque " << id <<" contains " << ret << " elements" << endl;
            }

            return ret;
        }

        void strdeque_insert_at(unsigned long id, size_t pos, const char *value) {
            //If the deque of the given id exists and value is not NULL, inserts value at the specific position or
            //at the end if the position is beyond the deque.
            //Otherwise does nothing.

            string toPrint;

            if(debug) {
                initStreams();
            }
            if(value == NULL) {
                if(debug) {
                    cerr << "strdeque_insert_at(" << id <<", " << pos << ", NULL)" << endl;
                    cerr << "strdeque_insert_at: attempt to insert NULL into a deque" << endl;
                }
                return;
            }
            if(debug) {
                toPrint += "strdeque_insert_at(";
                toPrint += to_string(id);
                toPrint += ", ";
                toPrint += to_string(pos);
                toPrint += ", \"";
                toPrint += value;
                toPrint += "\")\n";
            }

            MY_DEQUE_MAP::iterator it = deque_map().find(id);
            if (it == deque_map().end()) {
                if(debug) {
                    cerr << toPrint << "strdeque_insert_at: deque " << id << " does not exist" << endl;
                }
                return;
            }
            if((*it).second.first) {

                if(debug) {
                    cerr << "strdeque_insert_at(the Empty Deque, " << pos << ", \"" << value << "\")" << endl
                         << "strdeque_insert_at: the Empty Deque contains 0 elements" << endl;
                }
                return;
            }

            string tmpString(value);
            deque<string>* tmpDeque = (*it).second.second;
            auto placeToInsert = min(tmpDeque->begin() + pos, tmpDeque->end());
            unsigned long placeNo = distance(tmpDeque->begin(), placeToInsert);
            tmpDeque->insert(placeToInsert, tmpString);
            if(debug) {
                cerr << toPrint << "strdeque_insert_at: deque " << id <<", element \"" << value << "\" inserted at "
                     << placeNo << endl;
            }
        }

        void strdeque_remove_at(unsigned long id, size_t pos) {
            //If the deque of the given id exists and pos is not beyond the deque, deletes an element at the pos.
            //Otherwise does nothing.

            string toPrint;

            if(debug) {
                initStreams();
                toPrint += "strdeque_remove_at(";
                toPrint += to_string(id);
                toPrint += ", ";
                toPrint += to_string(pos);
                toPrint += ")\n";
            }

            MY_DEQUE_MAP::iterator it = deque_map().find(id);
            if (it == deque_map().end()) {
                if(debug) {
                    cerr << toPrint << "strdeque_delete: deque " << id << " does not exist" << endl;
                }
                return;
            }

            if((*it).second.first) {
                if(debug) {
                    cerr << "strdeque_remove_at(the Empty Deque, " << pos << ")" << endl
                         << "strdeque_remove_at: the Empty Deque contains 0 elements" << endl;
                }
                return;
            }

            deque<string>* tmpDeque = (*it).second.second;
            if(tmpDeque->size() <= pos) {
                if(debug) {
                    cerr << toPrint << "strdeque_remove_at: deque " << id << " does not containt an element at " << pos << endl;
                }
                return;
            }

            tmpDeque->erase(tmpDeque->begin() + pos);
            if(debug) {
                cerr << toPrint << "strdeque_remove_at: deque " << id <<", element at " << pos << " removed" << endl;
            }
        }

        const char *strdeque_get_at(unsigned long id, size_t pos) {
            //If the deque of the given id exists and pos is not beyond the deque, returns pointer to the position.
            //Otherwise returns NULL.

            string toPrint;

            if(debug) {
                initStreams();
                toPrint += "strdeque_get_at(";
                toPrint += to_string(id);
                toPrint += ", ";
                toPrint += to_string(pos);
                toPrint += ")\n";
            }
            MY_DEQUE_MAP::iterator it = deque_map().find(id);
            if (it == deque_map().end()) {
                if(debug) {
                    cerr << toPrint << "strdeque_get_at: deque " << id << " does not exist" << endl;
                }
                return NULL;
            }
            if((*it).second.first) {
                if(debug) {
                    cerr << "strdeque_get_at(the Empty Deque, " << pos << ")" << endl
                         << "strdeque_get_at: the Empty Deque does not contain an element at " << pos << endl;
                }
                return NULL;
            }
            deque<string> *tmp_deque = (*it).second.second;
            if(tmp_deque->size() <= pos) {
                if(debug) {
                    cerr << toPrint << "strdeque_get_at: deque " << id << " does not contain an element at " << pos << endl;
                }
                return NULL;
            }

            if(debug) {
                cerr << toPrint << "strdeque_get_at: deque " << id <<", element at " << pos << " is \""
                     << (*tmp_deque)[pos].c_str() << "\"" << endl;
            }
            return (*tmp_deque)[pos].c_str();
        }

        void strdeque_clear(unsigned long id) {
            //If the deque of the given id exists, deletes all its elements.
            //Otherwise does nothing.

            string toPrint;

            if(debug) {
                initStreams();
                toPrint += "strdeque_clear(";
                toPrint += to_string(id);
                toPrint += ")\n";
            }
            MY_DEQUE_MAP::iterator it = deque_map().find(id);
            if (it == deque_map().end()) {
                if(debug) {
                    cerr << toPrint << "strdeque_clear: deque " << id << " does not exist" << endl;
                }
                return;
            }
            if((*it).second.first) {

                if(debug) {
                    cerr << "strdeque_clear(the Empty Deque)" << endl << "strdeque_clear: attempt to clear the Empty Deque"
                         << endl;
                }
                return;
            }
            (*it).second.second->clear();
            if(debug) {
                assert((*it).second.second->size() == 0);
                cerr << toPrint << "strdeque_clear: deque " << id << " cleared" << endl;
            }
        }

        int strdeque_comp(unsigned long id1, unsigned long id2) {
            //Compares two deques in lexicographical order. If a deque doesn't exist, it is treated as empty deque.
            // -1, when deque(id1) < deque(id2),
            // 0, when deque(id1) = deque(id2),
            // 1, when deque(id1) > deque(id2).

            string toPrint;
            MY_DEQUE_MAP::iterator it1 = deque_map().find(id1);
            MY_DEQUE_MAP::iterator it2 = deque_map().find(id2);
            deque<string>::iterator istr1, istr2;

            if(debug) {
                initStreams();
                toPrint += "strdeque_comp(";
                toPrint += to_string(id1);
                toPrint += (*it1).second.first ? " (the Empty Deque)" : "";
                toPrint += ", ";
                toPrint += to_string(id2);
                toPrint += (*it2).second.first ? " (the Empty Deque)" : "";
                toPrint += ")\n";
                cerr << toPrint;
            }

            if(it1 == deque_map().end() && it2 == deque_map().end()) {
                if(debug) {
                    cerr << "strdeque_comp: Neither deque " << id1 << " nor " << id2 << " exists" << endl;
                }
                return 0;
            }

            if (it1 == deque_map().end()) {
                istr2 = (*it2).second.second->begin();
                while(istr2 != (*it2).second.second->end()) {
                    if(*istr2 != "") {
                        if(debug) {
                            cerr << "strdeque_comp: result of comparing deque " << id1
                                 << " that doesn't exist to deque " << id2 << " is -1 " << endl;
                        }
                        return -1;
                    }
                    istr2++;
                }

                if(debug) {
                    cerr << "strdeque_comp: result of comparing deque " << id1
                         << " that doesn't exist to deque " << id2 << " is 0 " << endl;
                }
                return 0;
            }
            if (it2 == deque_map().end()) {
                istr1 = (*it1).second.second->begin();
                while(istr1 != (*it1).second.second->end()) {
                    if(*istr1 != ""){
                        if(debug) {
                            cerr << "strdeque_comp: result of comparing deque " << id1 << " to deque " << id2
                                 << " that doesn't exist is 1 " << endl;
                        }
                        return 1;
                    }
                    istr1++;
                }

                if(debug) {
                    cerr << "strdeque_comp: result of comparing deque " << id1 << " to deque " << id2
                         << " that doesn't exist is 0 " << endl;
                }
                return 0;
            }
            string str1, str2;
            int cmp;
            istr1 = (*it1).second.second->begin();
            istr2 = (*it2).second.second->begin();
            while(istr1 != (*it1).second.second->end() || istr2 != (*it2).second.second->end()) {
                str1 = istr1 == (*it1).second.second->end() ? "" : (*istr1);
                str2 = istr2 == (*it2).second.second->end() ? "" : (*istr2);
                cmp = str1.compare(str2);
                if(cmp < 0) {
                    if(debug) {
                        cerr << "strdeque_comp: result of comparing deque " << id1 << " to deque " << id2
                             << " is -1 " << endl;
                    }
                    return -1;
                }
                if(cmp > 0) {
                    if(debug) {
                        cerr << "strdeque_comp: result of comparing deque " << id1 << " to deque " << id2
                             << " is 1 " << endl;
                    }
                    return 1;
                }
                if(istr1 != (*it1).second.second->end()) {
                    istr1++;
                }
                if(istr2 != (*it2).second.second->end()) {
                    istr2++;
                }
            }
            if(debug) {
                cerr << "strdeque_comp: result of comparing deque " << id1 << " to deque " << id2
                     << " is 0 " << endl;
            }
            return 0;
        }
    }
}