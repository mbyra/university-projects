/*
 * Zadanie 1 Znaczki
 * Marcin Byra mb347056
 * Mateusz Rubka mr371635
 */
#include <iostream>
#include <regex>

using namespace std;

/*
 * Name of stamp, price (display value), price (real value), year, name of the post office.
 */
typedef tuple<string, string, float, int, string> Stamp;

/*
 * Range of years.
 */
typedef pair<int, int> IntPair;

/*
 * Removes multiple spaces from string, used before comparing one stamp to another.
 */
string removeMultipleSpaces(string s);

/*
 * Converts string containing int to int.
 */
int stringToInt(const string s);

/*
 * Checks if number in string is a valid year number.
 */
bool isYearValid(const string s);

/*
 * Checks if number in string is a float number, normalises it.
 */
float normalisePrice(string s);

void parseInput(vector<Stamp> *stamps, vector<IntPair> *dates);

void processQueries(vector<Stamp> stamps, vector<IntPair> dates);

/*
 * Key used to sort stamps in vector.
 */
struct key_to_sort {
    inline bool operator()(const Stamp &row1, const Stamp &row2) {
        if (get<3>(row1) != get<3>(row2)) {
            return (get<3>(row1) < get<3>(row2));
        } else if (removeMultipleSpaces(get<4>(row1)) != removeMultipleSpaces(get<4>(row2))) {
            return removeMultipleSpaces(get<4>(row1)) < removeMultipleSpaces(get<4>(row2));
        } else if (get<2>(row1) != get<2>(row2)) {
            return (get<2>(row1) < get<2>(row2));
        } else {
            return removeMultipleSpaces(get<0>(row1)) < removeMultipleSpaces(get<0>(row2));
        }
    }
};

/*
 * Key used to search stamps in vector.
 */
struct key_to_search {
    int asValue(const Stamp &r) {
        return get<3>(r);
    }

    int asValue(int year) {
        return year;
    }

    template<typename T1, typename T2>

    inline bool operator()(const T1 &t1, const T2 &t2) {
        return (asValue(t1) < asValue(t2));
    }
};

int main() {
    vector<Stamp> stamps;
    vector<IntPair> dates;

    parseInput(&stamps, &dates);

    sort(stamps.begin(), stamps.end(), key_to_sort());

    processQueries(stamps, dates);

    return 0;
}

string removeMultipleSpaces(string s) {
    regex multiple_spaces = regex("[^[:graph:]]+");
    s = regex_replace(s, multiple_spaces, " ");
    return s;
}

int stringToInt(const string s) {
    stringstream ss(s);
    int output;
    ss >> output;
    return output;
}

void parseInput(vector<Stamp> *stamps, vector<IntPair> *dates) {
    string line;
    int line_number = 0;
    regex stamp("([^[:graph:]]*)(\\w.+[\\w])([^[:graph:]]+)(\\d*[,.]?\\d+)([^.]+)(\\d{4})([^\\w]+)(.+[\\w\\d])([^[:graph:]]*)$");
    regex date("([^[:graph:]]*)(\\d{4})([^[:graph:]]+)(\\d{4})([^[:graph:]]*)");
    smatch matches;
    bool end_of_stamps = false;

    while (getline(cin, line)) {
        if (!cin.bad() && !cin.eof()) {
            line_number++;
            if (!end_of_stamps && regex_search(line, matches, stamp)) {
                if (matches.size() == 10) {
                    if (!isYearValid(matches[6])) {
                        cerr << "Error in line " << line_number << ':' << line << endl;
                    } else {
                        stamps->push_back(Stamp(matches[2], matches[4], normalisePrice(matches[4]), stringToInt(matches[6]), matches[8]));
                    }
                } else {
                    cerr << "Error in line " << line_number << ':' << line << endl;
                }
            } else if (regex_search(line, matches, date)) {
                if (matches.size() == 6 && isYearValid(matches[2]) && isYearValid(matches[4]) &&
                    stringToInt(matches[2]) <= stringToInt(matches[4])) {
                    end_of_stamps = true;
                    dates->push_back(IntPair(stringToInt(matches[2]), stringToInt(matches[4])));
                } else {
                    cerr << "Error in line " << line_number << ':' << line << endl;
                }
            } else {
                cerr << "Error in line " << line_number << ':' << line << endl;
            }
        }
    }
}

bool isYearValid(const string s) {
    int year = stringToInt(s);
    return !(year <= 1000 || year >= 9999);
}

float normalisePrice(string s) {
    float output;
    size_t comma = s.find(',');
    if (comma != string::npos) {
        s[comma] = '.';
    }
    stringstream ss(s);
    ss >> output;
    return output;
}

void processQueries(vector<Stamp> stamps, vector<IntPair> dates) {
    for (size_t i = 0; i < dates.size(); i++) {
        vector<Stamp>::const_iterator end = upper_bound(stamps.begin(), stamps.end(), dates[i].second, key_to_search());
        for (vector<Stamp>::const_iterator j = lower_bound(stamps.begin(), stamps.end(), dates[i].first, key_to_search());
             j != end; j++) {
            cout << get<3>(*j) << ' ' << get<4>(*j) << ' ' << get<1>(*j) << ' ' << get<0>(*j) << endl;
        }
    }
}
