//#include <iostream>
//#include <vector>
//#include <tuple>
//#include <cassert>
//
//using std::vector;
//using std::cout;
//using std::endl;
//using std::tuple;
//using std::get;
//
//typedef struct node {
//    int l, r, sum, color;
//} node;
//
////Returns smallest power of 2 greater than given number
//int smallestPowerGreaterThan(int n);
//
//void parseInput(int&, vector<tuple<int,int,char>>&);
//
//void printVector(vector<node> tree);
//
//void printVector(vector<tuple<int,int,char>>);
//
//node emptyNode();
//
//int howManyWhite(const vector<node>& T);
//
//node createFirstNode(int l, int r, int c);
//
//int update(vector<node>& T, int i, int l, int r, int c);
//
//vector<node> initializeTree(int n);
//
//int main() {
//    int n;
//    vector<tuple<int,int,char>> input;
//    parseInput(n, input);
//
//    vector<node> T = initializeTree(n);
//
//    for(int i = 0; i < input.size(); i++) {
//        printf("%d\n", update(T, 1, get<0>(input[i]), get<1>(input[i]), get<2>(input[i])));
//        printVector(T);
//    }
//    return 0;
//}
//
//vector<node> initializeTree(int n) {
//    vector<node> T(2*smallestPowerGreaterThan(n), emptyNode());
//    //at the beginning all highway is painted black.
//    T[1] = createFirstNode(1, smallestPowerGreaterThan(n), 1);
//
//    for(int i = 2; i < T.size(); i++) {
//        if(i % 2 == 0) { //lewy syn
//            T[i].l = T[i/2].l;
//            T[i].r = T[i].l + (T[i/2].r - T[i/2].l + 1) / 2 - 1;
//        }
//        else {
//            T[i].r = T[i/2].r;
//            T[i].l = T[i].r - (T[i/2].r - T[i/2].l + 1) / 2 + 1;
//        }
//        T[i].sum = 0;
//    }
//
//    return T;
//}
//
//
//int update(vector<node>& T, int i, int l, int r, int c) {
//    cout << "update(i = " << i <<", l = " << l <<", r = " << r << ", c = " << c << "). T[i].l = " << T[i].l << ", T[i].r = " << T[i].r << endl;
//    if(T[i].r < l || T[i].l > r) {
//        cout << "przedzialy rozlaczne" << endl;
////        przedzialy T[i] i (l,r) rozlaczne
//        if(T[i/2].color == -1) {
//            T[i].color =-1;
//            return T[i].r - T[i].l + 1;
//        }
//        else if(T[i/2].color == 1){
//            T[i].color = 1;
//            return 0;
//        }
//        else return T[i].sum;
//    }
//    else if(T[i].l >= l && T[i].r <= r) {
//        cout << "przedzial wewnatrz" << endl;
//        //przedzial wezla jest "wewnatrz" (l,r) - maluje caly wezel
//        T[i].color = c;
//        T[i].sum = c == -1 ? T[i].r - T[i].l + 1 : 0;
//
//    }
//    else {
//        cout << "przedzialy mieszane, schodze nizej" << endl;
//        //przedzialy wezla i do pomalowania "zahaczaja sie"
//        T[i].sum = update(T, 2*i, l, r, c) + update(T, 2*i + 1, l, r, c);
//        T[i].color = 0; // przedzial bedzie mieszany
//    }
//    cout << "update(i = " << i << "), zwracam " << T[i].sum << endl;
//    return T[i].sum;
//}
//
//node createFirstNode(int l, int r, int c) {
//    node node;
//    assert(r >= l);
//    node.l = l;
//    node.r = r;
//    node.sum = 0;
//    node.color = c;
//    return node;
//}
//
//int howManyWhite(const vector<node>& T) {
//    return T[0].sum;
//}
//
//node emptyNode() {
//    node node;
//    node.l = 0;
//    node.r = 0;
//    node.sum = 0;
//    node.color = 1;
//
//    return node;
//}
//
//void printVector(vector<node> tree) {
//    for(int i = 1; i < tree.size(); i++) {
//        cout << "T[" << i << "] : l = " << tree[i].l << ", r = " << tree[i].r << ", c = " << tree[i].color << endl;
//    }
//}
//
//void printVector(vector<tuple<int,int,char>> input) {
//    for(int i = 0; i < input.size(); i++) {
//        cout << get<0>(input[i]) << " " << get<1>(input[i]) << " " << get<2>(input[i]) << endl;
//    }
//}
//
//void parseInput(int& n, vector<tuple<int,int,char>>& input) {
//    int m;
//    char tmp;
//    scanf("%d", &n);
//    scanf("%d", &m);
//    assert(n > 0 && n < 1000001 && m > 0 && m < 1000001);
//    int l, r;
//    char c;
//
//    for(int i = 0; i < m; i++) {
//        scanf("%d", &l);
//        scanf("%d", &r);
//        scanf("%c", &tmp); //space
//        scanf("%c", &c);
//        input.push_back(std::make_tuple(l,r, c == 'B' ? -1 : 1));
//    }
//}
//
//int smallestPowerGreaterThan(int n) {
//    int power = 1;
//    while(power < n) {
//        power*=2;
//    }
//    return power;
//}

#include <iostream>


const long maxN = 2 * 1000000;
long n;
long T[maxN * 2][2];
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

long insert(long p, long k, long ip, long ik, long i, long w) {
    if (ip == ik) {
        T[i][0] = w;
        T[i][1] = 1;
        return T[i][0];
    }

    if ((p == ip) && (k == ik)) {
        if (w*(k - p + 1) == T[i][0])
            return T[i][0];
        T[i][0] = w*(k - p + 1);
        T[i][1] = 1;
        return T[i][0];
    }

    if (T[i][1] == 1) {
        T[i * 2][0] = T[i][0] / 2;
        T[i * 2 + 1][0] = T[i][0] / 2;
        T[i][1] = 0;
        T[i * 2][1] = 1;
        T[i * 2 + 1][1] = 1;
    }
    long s = (ip + ik) / 2;
    if (p <= s)
        insert(p, min(k, s), ip, s, i * 2, w);

    if (k > s)
        insert(max(p, s + 1), k, s + 1, ik, i * 2 + 1, w);

    T[i][0] = T[i * 2][0] + T[i * 2 + 1][0];
    return 0;
}
long max;
void insert(long p, long k, long w) {
    insert(p, k, 1, max, 1, w);
}

int main() {
    long k, p1, p2;

    char c;
    std::cin >> n >> k;
    max = 2;
    while (max < 2 * n)
        max *= 2;
    for (long i = 0; i < k; i++) {
        std::cin >> p1 >> p2 >> c;
        insert(p1, p2, c == 'C' ? 0 : 1);

        std::cout << T[1][0] << std::endl;
    }
    return 0;
}
