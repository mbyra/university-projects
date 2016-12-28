#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <forward_list>
#include <queue>

using namespace std;

typedef std::tuple<int, int, int, std::forward_list<int>, int> island; // <x, y, no, list_of_neighbours, distance>

int dist(island l, island r)
{
    return std::min(std::abs(std::get<0>(l) - std::get<0>(r)), std::abs(std::get<1>(l) - std::get<1>(r)));
}

int main()
{
    std::vector<island>::size_type n; // number of islands
    std::cin >> n;

    std::vector<island> islands(n);

    for (auto i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        islands[i] = std::make_tuple(x, y, i, std::forward_list<int>(), 1000000000 + 1);
    }
    std::get<4>(islands[0]) = 0;

    // sort by x:
    std::sort(islands.begin(), islands.end(),
              [] (const island& lhs, const island & rhs)
              {
                  if (get<0>(lhs) != get<0>(rhs)) // sort by x...
                      return get<0>(lhs) < get<0>(rhs);
                  else
                      return get<1>(lhs) < get<1>(rhs); // then by y
              });

    // connect closest neighbours by x: (add numbers of x-neighbours to neighbour list
    get<3>(islands[0]).push_front((int &&) get<2>(islands[1]));
    for(int i = 1; i < n - 1; i++) {
        get<3>(islands[i]).push_front((int &&) get<2>(islands[i - 1]));
        get<3>(islands[i]).push_front((int &&) get<2>(islands[i + 1]));
    }
    get<3>(islands[n - 1]).push_front((int &&) get<2>(islands[n - 2]));

    // sort by y:
    std::sort(islands.begin(), islands.end(),
              [] (const island& lhs, const island & rhs)
              {
                  if (get<1>(lhs) != get<1>(rhs)) // sort by y...
                      return get<1>(lhs) < get<1>(rhs);
                  else
                      return get<0>(lhs) < get<0>(rhs); // then by x
              });



    // connect closest neighbours by y: (add numbers of y-neighbours to neighbour list
    get<3>(islands[0]).push_front((int &&) get<2>(islands[1]));
    for(int i = 1; i < n - 1; i++) {
        get<3>(islands[i]).push_front((int &&) get<2>(islands[i - 1]));
        get<3>(islands[i]).push_front((int &&) get<2>(islands[i + 1]));
    }
    get<3>(islands[n - 1]).push_front((int &&) get<2>(islands[n - 2]));


    // sort islands by their original order (by number)
    std::sort(islands.begin(), islands.end(),
              [] (const island& lhs, const island& rhs)
              {
                  return get<2>(lhs) < get<2>(rhs);
              });


    // dijkstra: shortest single source path where weigh (distance) is minimum of (x_difference, y_difference)
    // from island 0 to island n - 1

    std::priority_queue<
            std::pair<int, int>,
            std::vector<std::pair<int, int>>,
            std::greater<std::pair<int, int>>> queue;

    queue.push(std::make_pair(0, 0)); // <dist, island_no>

    while (!queue.empty()) {
        auto w = queue.top();
        queue.pop();

        auto pom = islands[w.second];
        for (auto const i : std::get<3>(pom)) {
            int pom2 = std::get<4>(pom) + dist(islands[i], islands[w.second]);
            if (pom2 < std::get<4>(islands[i])) {
                std::get<4>(islands[i]) = pom2;
                queue.push(std::make_pair(pom2, i));
            }
        }
    }

    std::cout << std::get<4>(islands[n - 1]) << std::endl;

}

