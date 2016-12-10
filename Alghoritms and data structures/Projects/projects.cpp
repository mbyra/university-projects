#include <iostream>
#include <vector>
#include <forward_list>
#include <queue>

int main() {
    size_t n; // number of projects
    int m; // number of dependencies
    int k; // minimal number of projects

    scanf("%zu", &n);
    scanf("%d", &m);
    scanf("%d", &k);

    std::vector<int> neededProgrammers(n); //weight of a node
    std::vector<std::forward_list<int>> dependencies(n);//list of project accessible only after completing this project,
                                                        //in other words,list of nodes accessible from this one (outDeg)
    std::vector<int> inDeg(n); //input degree of each project, i.e. how many project are needed before starting this one

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>>> pq;

    std::vector<int> biggest(n); //biggest of best fitting projects; for start it's a list of roots in project graph

    for(int i = 0; i < n; i++) {
        int tmp;
        scanf("%d", &tmp);
        neededProgrammers[i] = tmp;
    }

    //create information about dependencies between nodes: how large is "inputDegree" of node and, if the node is
    //dependent from others, adding it to their lists
    for(int i = 0; i < m; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        inDeg[a - 1]++; // -1 because array's indexed from 0 and project from input from 1
        dependencies[b - 1].push_front(a - 1); //b - 1 must be done before starting a - 1 so I add a - 1 to b - 1's list
    }


    //Looking for roots of the graph
    for(int i = 0; i < n; i++) {
        if(inDeg[i] == 0) {
            pq.push(std::make_pair(neededProgrammers[i], i));
            biggest[i] = neededProgrammers[i]; //this node is root. It's weigh is neededProgrammers[i].
        }
        else {
            biggest[i] = 0; //this node is not root
        }
    }

    //We want to realize only k projects (starting from 1, thus k - 1)
    for(int i = 0; i < k - 1; i++) {
        const std::pair<int, int> v = pq.top(); // copying node from top before pop
        pq.pop();
        for(const int u : dependencies[v.second]) { //for every node that depends on v...
            inDeg[u]--; //forgetting the edge from v to u, thus u has 1 less input edge.
            //the biggest of chosen project can remain the same, be the biggest starting from v, or the u itself:
            biggest[u] = std::max(std::max(biggest[u], biggest[v.second]), neededProgrammers[u]);
            if(inDeg[u] == 0) { //if I have no input edge, I can be the one to check next
                pq.push(std::make_pair(biggest[u], u));

            }
        }
    }

    //Printing minimal number of programmers to realize k projects:
    printf("%d\n",pq.top().first);

}