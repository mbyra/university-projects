#include <iostream>
#include <vector>
#include <cassert>

using std::vector;
using std::cout;
using std::endl;

//returns m x n vector filled with values from standard input
vector<vector<int>> parseInput();
//debugging purpose
void print2dVector(const vector<vector<int>>&);
//Creates 4d matrix m x n x n x 3 initialized with 0s. Additional rows and cols filled with zeros to avoid
//checking condition of reaching out of data matrix.
vector<vector<vector<vector<int>>>> createMAXMatrix(const vector<vector<int>>& values);
//Finds solution, i.e. best possible score and prints it to standard output.
int findBestResult(const vector<vector<int>>& values);
//Finds value for MAX[i][j][k][0:2] i.e. greatest number of points until stepped on points (i,j) and (j, k).
void fillMAX(vector<vector<vector<vector<int>>>>& MAX, const vector<vector<int>>& values, const int i, const int j,
             const int k);


int main() {

    vector<vector<int>> values = parseInput();
    printf("%d", findBestResult(values));

    return 0;
}

int getMAX(vector<vector<vector<vector<int>>>>& MAX, const int i, const int jj, const int kk, const int level) {
    if(i < 0 || jj < 0 || jj >= MAX[0].size() || kk < 0 || kk >= MAX[0].size())
        return 0;
    else
        return MAX[i][jj][kk][level];
}

void fillMAX(vector<vector<vector<vector<int>>>>& MAX, const vector<vector<int>>& values, const int i, const int j,
             const int k) {
    //(i,j) represents 1-level-step path (can be reached from points (i-1, j-2:j+2)
    //(i,k) represents 2-or-3-level-step path (can be reached from points (i-3:i-2, k-1:k+2)
    //thus it's necessary to find maximal of these points as maximal possible score so far.
    int max0 = 0, max1 = 0, max2 = 0;
    int maxCombination;
    int maxFromLevelBefore;
    int maxFrom2LevelsBefore;

    //Taking all possible combinations of points
    for(int jj = j - 2; jj <= j + 2; jj++) {
        for(int kk = k - 1; kk <= k + 1; kk++) {
            for(int level = 1; level <= 2; level++) {
                maxCombination = getMAX(MAX, i-1, jj, kk, level);
                if(maxCombination > max0 && j != k) max0 = maxCombination;
            }
        }
        //At the same time finding maximums of 1 and 2 levels before
        maxFromLevelBefore = getMAX(MAX, i-1, jj, k, 0);
        if(maxFromLevelBefore > max1 && jj != k) max1 = maxFromLevelBefore;
        maxFrom2LevelsBefore = getMAX(MAX, i-1, jj, k, 1);
        if(maxFrom2LevelsBefore > max2) max2 = maxFrom2LevelsBefore;

    }

    MAX[i][j][k][0] = values[i][j] + values[i][k];
    //To avoid assigning values from input to level 1 and 2 in first row.
    if(i - 1 >= 0) {
        MAX[i][j][k][1] = values[i][j];
        MAX[i][j][k][2] = values[i][j];
    }

    MAX[i][j][k][0] += max0;
    MAX[i][j][k][1] += max1;
    MAX[i][j][k][2] += max2;
}

//values matrix is input matrix
int findBestResult(const vector<vector<int>>& values) {
    vector<vector<vector<vector<int>>>> MAX = createMAXMatrix(values);

    for(int i = 0; i < values.size(); i++) { //For each row...
        for(int j = 0; j < values[0].size(); j++) { //...for each point in a row...
            for(int k = 0; k < values[0].size(); k++) { ///...paired with every other point in that row...
                //...fills the MAX matrix with maximal reachable until i-th level if (i,j) and (i,k) points are chosen
                    fillMAX(MAX, values, i, j, k);
            }
        }
    }

    //After all calculation, I just have to find highest score in last row.
    int max = 0;
    for(int j = 0; j < values[0].size(); j++) {
        for(int k = 0; k < values[0].size(); k++) {
            if(MAX[values.size()-1][j][k][0] > max) {
                max = MAX[values.size()-1][j][k][0];
            }
        }
    }

    return max;
}

vector<vector<vector<vector<int>>>> createMAXMatrix(const vector<vector<int>>& A) {
    //TODO I don't know why clear 4d initialization with given dimensions and zeros was unable. However, I bypassed it
    vector<vector<vector<vector<int>>>> MAX;
    for(int i = 0; i < A.size(); i++) {
        vector<vector<vector<int>>> temp(A[0].size(), vector<vector<int>>(A[0].size(), vector<int>(3,0)));
        MAX.push_back(temp);
    }

    return MAX;
}

vector<vector<int>> parseInput() {

    char eol;
    int m, n, temp;
    vector<int> tempRow;
    vector<vector<int>> values;

    std::scanf("%d", &m);
    std::scanf("%d", &n);
    std::scanf("%c", &eol);
    assert(m >= 3 && n <= 500 && n >= 2 && n <= 40 && eol == '\n');

    tempRow.reserve(n);
    values.reserve(m);
    for(auto &i: values) i.reserve(n);

    for(size_t i = 0; i < m; i++) {
        tempRow.clear();
        for(size_t j = 0; j < n; j++) {
            scanf("%d", &temp);
            tempRow.push_back(temp);
        }
        values.push_back(tempRow);
        std::scanf("%c", &eol);
    }

    return values;
}

void print2dVector(const vector<vector<int>>& A) {
    for(const auto &i : A) {
        for(const auto &j : i) {
            cout << j << " ";
            if(j < 10) cout << " ";
        }
        cout << endl;
    }
}
