#include <iostream>
#include <assert.h>

int main() {
    int n;
    int *input;
    long **Cl;
    long **Cr;
    char eol;

    scanf("%d", &n);
    scanf("%c", &eol);
    assert(eol == '\n');

    if(n == 1) {
        printf("1");
        return 0;
    }

    input = new int[n];
    for(int i = 0; i < n; i++) {
        scanf("%d", input + i);
    }
//    for(int i = 0; i < n; i++) {
//        printf("%d\n", input[i]);
//    }

    Cl = new long*[n];
    Cr = new long*[n];
    for(int i = 0; i < n; i++) {
        Cl[i] = new long[n];
        Cr[i] = new long[n];
    }


    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Cl[i][j] = Cr[i][j] = 0;
        }
    }


    int j;
    for(int k = 1; k < n; k++) {
        for(int i = 0; i < n - k; i++) {
            j = i + k;
            if(k == 1 && input[j] > input[i])
                Cl[i][j] = Cr[i][j] = 1;
            else { /* j > i + 1 */
                if(input[i+1] > input[i])
                    Cl[i][j] += Cl[i+1][j] % 1000000000;
                if(input[j] > input[i])
                    Cl[i][j] += Cr[i+1][j]% 1000000000;
                if(input[j] > input[i])
                    Cr[i][j] += Cl[i][j-1]% 1000000000;
                if(input[j] > input[j-1])
                    Cr[i][j] += Cr[i][j-1]% 1000000000;
                Cl[i][j] = Cl[i][j] % 1000000000;
                Cr[i][j] = Cr[i][j] % 1000000000;

            }
        }
    }


//    for(int i = 0; i < n; i++) {
//        for (int j = 0; j < n; j++) {
//            printf("%d", Cl[i][j]);
//        }
//        printf("\n");
//    }
//    printf("\n\n");
//
//    for(int i = 0; i < n; i++) {
//        for (int j = 0; j < n; j++) {
//            printf("%d", Cr[i][j]);
//        }
//        printf("\n");
//    }
//    printf("\n\n");


    printf("%d", (Cl[0][n-1] + Cr[0][n-1]) % 1000000000);

    for(int i = 0; i < n; i++) {
        delete[] Cl[i];
        delete[] Cr[i];
    }
    delete[] Cl;
    delete[] Cr;
    delete[] input;

    return 0;
}
