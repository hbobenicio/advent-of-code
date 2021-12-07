#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#define WINDOW_LENGTH 4

bool array_contains(const char* s, size_t s_len, char c) {
    for (size_t i = 0; i < s_len; i++) {
        if (s[i] == c) {
            return true;
        }
    }
    return false;
}

int main() {
    char datastream[8 * 1024] = {0};
    char window[WINDOW_LENGTH] = {0};
    size_t window_len = 0;
    assert(scanf("%s", datastream) == 1);
    for (size_t i = 0; i < strlen(datastream); i++) {
        char c = datastream[i];
        if (array_contains(window, window_len, c)) {
            memset(window, 0, WINDOW_LENGTH * sizeof(window[0]));
            window_len = 0;
        }
        window[window_len++] = c;
        if (window_len == WINDOW_LENGTH) {
            printf("%zu\n", i + 1);
            break;
        }
    }
}

