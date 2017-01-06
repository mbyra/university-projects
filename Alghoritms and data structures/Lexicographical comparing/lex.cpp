#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using std::vector;

// Format of query:
typedef std::pair<std::pair<int, int>, std::pair<int, int>> quartet;

// Pair of identifiers, the collection of which we will sort. To save resulting hash it is needed to remember index of
// word which is represented by given pair.
struct Pair
{
    int p1, p2, index;
};

// Returns an integer which is greatest p for which 2^p <= n;
int greatest_power_of_2_less_equal_than(const int n)
{
    assert(n != 0);
    int i = 1;
    int p = 0;
    while (i * 2 <= n) {
        i *= 2;
        ++p;
    }

    return p;
}

// Function building a dictionary of base subwords (don't know if it's proper translation...)
// Returns 2-d array of subwords identifiers of given word s.
vector<vector<int>> build_KMR(const std::string& s)
{
    vector<vector<int>> ids;
    vector<int> curr_id(s.size());
    vector<Pair> pairs;

    // hashes of words of length of 1 are codes of alphabet symbols:
    for (int i = 0; i < s.size(); ++i) {
        curr_id[i] = (int) s[i];
    }
    ids.push_back(curr_id);

    int p = 2;
    while (p < s.size()) { // for next powers of 2
        curr_id.resize(s.size() - p + 1);
        pairs.resize(s.size() - p + 1);

        // hash of every word of length 2, 4, 8... starting from i consists of two codes,
        // i.e. hashes of its half-words
        for (int i = 0; i < s.size() - p + 1; ++i) {
            pairs[i].p1 = ids.back()[i];
            pairs[i].p2 = ids.back()[i + p/2];
            pairs[i].index = i;
        }

        // sort pairs of identifiers: TODO bucket, radix - linear, not O(nlogn)
        std::sort(pairs.begin(), pairs.end(),
                  [](const Pair& lhs, const Pair& rhs)
                  {     if (lhs.p1 != rhs.p1)
                            return lhs.p1 < rhs.p1;
                        else
                            return lhs.p2 < rhs.p2; });

        // assign new identifiers: pairs with same first hash have same identifier
        int new_id = 0;
        for (int i = 0; i < pairs.size(); ++i) {
            if (i > 0 && (pairs[i - 1].p1 != pairs[i].p1 || pairs[i - 1].p2 != pairs[i].p2))
                ++new_id;
            curr_id[pairs[i].index] = new_id;
        }
        ids.push_back(curr_id); // we created unique identifiers for subwords of length p.
        p *= 2;
    }

    return ids;
}

// Returns a char representing if first subword (limited by letter indices in fist pair of quartet) is lexicographically
// less ('<'), equal ('=') or greater ('>') than second subword (limited by letter indices in second pair of quartet)
const char compare_subwords(quartet& q, const vector<vector<int>>& ids)
{
    int first_word_length = q.first.second - q.first.first + 1;
    int second_word_length = q.second.second - q.second.first + 1;

    assert(first_word_length > 0 && second_word_length > 0);

    if (first_word_length < second_word_length) {
        // Either words differ somewhere before first word ends or first is a subword of second.
        q.second.second = q.second.first + first_word_length - 1;
        char sign = compare_subwords(q, ids);

        return sign == '=' ? '<' : sign;
    }
    else if (first_word_length > second_word_length) {
        // Either words differ somewhere before second word ends or second is a subword of first.
        q.first.second = q.first.first + second_word_length - 1;
        char sign = compare_subwords(q, ids);

        return sign == '=' ? '>' : sign;
    }
    else {
        // words of the same length - I just have to compare greatest prefixes and suffixes which length is power of 2:
        int power = greatest_power_of_2_less_equal_than(first_word_length);
        int subword_length = pow(2, power);

        if (ids[power][q.first.first] != ids[power][q.second.first])
            return ids[power][q.first.first] < ids[power][q.second.first] ? '<' : '>';

        else { // only checking if the suffixes are equal.
            int first_suffix_begin = q.first.first + first_word_length - subword_length;
            int second_suffix_begin = q.second.first + second_word_length - subword_length;

            if (ids[power][first_suffix_begin] != ids[power][second_suffix_begin])
                return ids[power][first_suffix_begin] < ids[power][second_suffix_begin] ? '<' : '>';
            return '=';
        }
    }
}

int main()
{
    std::string::size_type n; // length of word
    vector<quartet>::size_type m; // number of queries
    std::string word;

    // parsing input:
    std::cin >> n >> m >> word;
    vector<quartet> queries(m);
    // read <a1, a2, b1, b2>, where a,b means the subword to compare, and 1 and 2 are first and last letters of subword
    for (auto& query :queries) {
        std::cin >> query.first.first >> query.first.second >> query.second.first >> query.second.second;
        // queries are indexed from 1, not from 0:
        query.first.first--;
        query.first.second--;
        query.second.first--;
        query.second.second--;
    }

    // pre-computation. two-dimensional array of word identifiers:
    vector<vector<int>> ids = build_KMR(word);


    // constant-time response:
    for (auto query : queries)
        std::cout << compare_subwords(query, ids) << std::endl;

}
