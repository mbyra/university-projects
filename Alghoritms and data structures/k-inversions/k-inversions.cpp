#include <iostream>
#include <vector>
#include <cassert>

using std::vector;

//Used to get values modulo "rest"
static const int rest = 1000000000;

//Reads n, k and array A from standard input assuming values are valid.
void parseInput(unsigned& n, unsigned& k, vector<unsigned>& A);

//Returns smallest power of 2 greater than given number
unsigned int smallestPowerGreaterThan(unsigned int n);

//Returns sum of elements in tree T[k] between a and b (both inclusive),n - number of elements in array(starting from 1)
unsigned long query(vector<vector<unsigned long>>& T, int k, int a, int b, int n);

//Increases value representing x-th element of array A in tree T[k] by value, n - numb. of el. in array(starting from 1)
void update(vector<vector<unsigned long>>& vector, int k, int x, unsigned long val, int n);

int main() {
    unsigned n, k;
    vector<unsigned> A;

    parseInput(n, k, A);

    vector<vector<unsigned long>> T(k + 1, vector<unsigned long>(2*smallestPowerGreaterThan(n), 0L));

    for(int i = 0; i < n; i++) {
        int x = A[i];
        update(T, 1, x, 1, n);

        for(int j = 2; j <= k; j++) {
            update(T, j, x, query(T, j-1, x+1, n, n), n);
        }
    }

    printf("%ld", T[k][1]);
    return 0;
}

void update(vector<vector<unsigned long>>& T, int k, int x, unsigned long val, int n) {
    int v = n + x -1;
    T[k][v] = T[k][v] % rest + val % rest;
    while(v != 1) {
        v /= 2;
        T[k][v] = (T[k][2*v] + T[k][2*v + 1]) % rest;
    }
}

unsigned long query(vector<vector<unsigned long>>& T, int k, int a, int b, int n) {
    if(a > b) {
        return 0;
    }
    int va = n + a - 1, vb = n + b - 1;
    unsigned long wyn = T[k][va] % rest;
    if(va != vb) {
        wyn = (wyn + T[k][vb]) % rest;
    }
    while(va/2 != vb/2) {
        if(va % 2 == 0) wyn = wyn % rest + T[k][va + 1] % rest;
        if(vb % 2 == 1) wyn = wyn % rest + T[k][vb - 1] % rest;
        va /= 2;
        vb /= 2;
    }
    return wyn % rest;
}


unsigned smallestPowerGreaterThan(unsigned int n) {
    unsigned power = 1;
    while(power < n) {
        power*=2;
    }
    return power;
}

void parseInput(unsigned& n, unsigned& k, vector<unsigned>& A) {

    char eol;
    unsigned temp;
    std::scanf("%d", &n);
    std::scanf("%d", &k);
    std::scanf("%c", &eol);
    assert(n >= 1 && n <= 20000 && k >=2 && k <= 10 && eol == '\n');

    A.reserve(n);
    for(int i = 0; i < n; i++) {
        std:scanf("%d", &temp);
        A.push_back(temp);
    }
}

