#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

void parse1stRow(unsigned &n, unsigned &m, char &eol) {
    scanf("%d", &n);
    scanf("%d", &m);
    scanf("%c", &eol);

    assert(n >= 4 && n <= 40000 && n % 2 == 0 && m >= 1 && m <= 50 && eol == '\n');
}

int main() {
    unsigned n,m;
    char eol;
    int player_no;

    parse1stRow(n, m, eol);

    std::vector<long long> players(n,0LL);

    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            scanf("%d", &player_no);
            if(j < n/2) players[player_no-1] += 1LL << i;
        }
        scanf("%c", &eol);
    }
    std::sort(players.begin(), players.end());

    players.erase(unique(players.begin(), players.end()), players.end());
    
    if(players.size() < n) {
        printf("NIE");
    }
    else {
        printf("TAK");
    }

    return 0;
}
