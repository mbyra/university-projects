#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <limits.h>

// using namespace std;

int main() {
    char c;
    char prev_letter = NULL;
    bool first = true;
    bool only_one = true;
    int size = 0;
    int min_stars = INT_MAX;
    int cur_stars = 0;

    while(scanf("%c", &c) != EOF) {
        if(c == '\n') {
            break;
        }
        size++;
        if(c == '*') {
            cur_stars++;
        }
        else if(only_one) {
            if(first) {
                first = false;
            }
            if(prev_letter != NULL && c != prev_letter) {
                only_one = false;
                min_stars = cur_stars < min_stars ? cur_stars : min_stars;
            }
            cur_stars = 0;
            prev_letter = c;
        }
        else if(c == prev_letter){
            cur_stars = 0;
        }
        else { /* c is a letter other than previous */
            min_stars = cur_stars < min_stars ? cur_stars : min_stars;
            cur_stars = 0;
            prev_letter = c;
        }
    }

    first || only_one ? printf("1") : printf("%d", size - min_stars);

    return 0;
}
