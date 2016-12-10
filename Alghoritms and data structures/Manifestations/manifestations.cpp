#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <queue>

using std::cout;
using std::endl;
using std::vector;
using std::tuple;
using std::make_tuple;
using std::get;

typedef std::vector<tuple<int, int, int>> input_vec;

struct {
    bool operator() (tuple<int, int> a, tuple<int,int> b) {
        if(get<0>(a) != get<0>(b)) return get<0>(a) < get<0>(b);
        else return get<1>(a) > get<1>(b);
    }
} tupleComparatorFirstThenSecond;

//Reads all input into proper variables; also prepares extra vectors containing:
//sorted recordings times, sorted entering and leaving tuples <time, manif_no>
//Assuming correctness of input data.
void parseInputAndPrepare(size_t &citizens_amount, size_t &recordings_amount, input_vec &citizens,
                          vector<int> &recordings, vector<int> &recordings_sorted,
                          vector<tuple<int, int>> &arriving, vector<tuple<int, int>> &leaving) {

    using std::sort;
    //Read how many citizens and record queries there will be:
    scanf("%zu", &citizens_amount);
    scanf("%zu", &recordings_amount);

    //Parse input to vector holding citizen enter (t0) and leave time (tend) and index of manifestation he takes part in
    int t0, tend, manif_no;
    for(size_t i = 0; i < citizens_amount; i++) {
        scanf("%d", &t0);
        scanf("%d", &tend);
        scanf("%d", &manif_no);
        citizens.push_back(make_tuple(t0, tend, manif_no));
        arriving.push_back(make_tuple(t0, manif_no));
        leaving.push_back(make_tuple(tend, manif_no));
    }

    sort(arriving.begin(), arriving.end(), tupleComparatorFirstThenSecond);
    sort(leaving.begin(), leaving.end(), tupleComparatorFirstThenSecond);

    //Parse input to vector holding the time of query which manifestation to record:
    int time;
    for(size_t i = 0; i < recordings_amount; i++) {
        scanf("%d", &time);
        recordings.push_back(time);
    }

    recordings_sorted = recordings;
    sort(recordings_sorted.begin(), recordings_sorted.end());

}

int main() {

    size_t citizens_amount, recordings_amount; // how many citizens and how many record queries
    input_vec citizens; // rows containing arrive and leave time and manifestation number
    vector<int> recordings_original, recordings_sorted; //rows containing queries of recording at given time
    vector<tuple<int,int>> arriving, leaving; // <arrive/leave time, manifestation_no>
    vector<int> manifestations(1000000, 0); // amount of people on i-th manifestation
    vector<tuple<int,int>> recordings_answers(1000000); // <time, biggest_manifestation_no>

    parseInputAndPrepare(citizens_amount, recordings_amount, citizens, recordings_original, recordings_sorted, arriving,
                         leaving);

    //tuple<amount_of_participants, manifestation_no>
    std::priority_queue<tuple<int, int>, vector<tuple<int,int>>, decltype(tupleComparatorFirstThenSecond)> pq;
    for(int i = 1; i <= 1000000; i++) {
        pq.push(make_tuple(0, i));
    }

    int start_time = std::min(get<0>(arriving[0]), recordings_sorted[0]);
    int end_time = std::max(get<0>(arriving[citizens_amount - 1]), recordings_sorted[recordings_amount - 1]);
    int time = start_time;
    int arriving_index = 0, leaving_index = 0, recording_index = 0;
    while(time <= end_time) {

        //Processing all the people arriving at given time
        while(get<0>(arriving[arriving_index]) == time) {
            int manif_no = get<1>(arriving[arriving_index]);
            manifestations[manif_no]++;
            pq.push(make_tuple(manifestations[manif_no], manif_no));
            arriving_index++;
        }

        //Processing all record request at given time
        while(recordings_sorted[recording_index] == time) {
            //popping outdated entries in queue connected by comparing with current numbers in manifestations[]
            while(manifestations[get<1>(pq.top())] != get<0>(pq.top())) {
                pq.pop();
            }
            //now the biggest manifestation is on the top of the queue
            recordings_answers[time] = make_tuple(get<1>(pq.top()), get<0>(pq.top()));
            recording_index++;
        }

        //Processing all the people leaving at given time
        while(get<0>(leaving[leaving_index]) == time) {
            int manif_no = get<1>(leaving[leaving_index]);
            manifestations[manif_no]--;
            pq.push(make_tuple(manifestations[manif_no], manif_no));
            leaving_index++;
        }


        time++;
    }

    //Creating output in original request order
    for(auto const& i : recordings_original) {
        cout << get<0>(recordings_answers[i]) << " " << get<1>(recordings_answers[i]) << endl;
    }

}