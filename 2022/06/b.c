#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#define WINDOW_LENGTH 14

typedef struct {
    char items[WINDOW_LENGTH];
    size_t len;
} NaiveSet;

bool array_contains(const char* s, size_t s_len, char c) {
    for (size_t i = 0; i < s_len; i++) {
        if (s[i] == c) {
            return true;
        }
    }
    return false;
}

NaiveSet naive_set_create(const char* s, size_t s_len) {
    NaiveSet set = {0};
    for (size_t i = 0; i < s_len; i++) {
        if (!array_contains(set.items, set.len, s[i])) {
            set.items[set.len++] = s[i];
        }
    }
    return set;
}

void array_print(const char* s, size_t s_len) {
    printf("[");
    for (size_t i = 0; i < s_len; i++) {
        if (i > 0) printf(", ");
        printf("%c", s[i]);
    }
    printf("] (%zu)\n", s_len);
}

int main() {
    char datastream[8 * 1024] = {0};
    assert(scanf("%s", datastream) == 1);
    for (size_t i = 0; i < strlen(datastream) - WINDOW_LENGTH + 1; i++) {
        NaiveSet set = naive_set_create(datastream + i, WINDOW_LENGTH);
        if (set.len == WINDOW_LENGTH) {
            printf("%zu\n", i + WINDOW_LENGTH);
            break;
        }
    }
}

