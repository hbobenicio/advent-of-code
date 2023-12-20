#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

const char* numbers_parse(const char* s, int64_t** xs, int64_t* len);

int64_t extrapolate(int64_t* xs, int64_t xs_count)
{
    for (int64_t j = xs_count - 1; j > 0; j--) {
        bool all_zeroes = true;
        for (int64_t i = 0; i < j; i++) {
            xs[i] = xs[i + 1] - xs[i];
            if (xs[i] != 0) {
                all_zeroes = false;
            }
        }
        if (all_zeroes) {
            break;
        }
    }
    for (int64_t i = 0; i < xs_count - 1; i++) {
        xs[i + 1] += xs[i];
    }
    return xs[xs_count - 1];
}

int64_t extrapolate_backwards(int64_t* xs, int64_t xs_count)
{
    for (int64_t i = 0; i < xs_count - 1; i++) {
        bool all_zeroes = true;
        for (int64_t j = xs_count - 1; j > i; j--) {
            xs[j] -= xs[j - 1];
            if (xs[j] != 0) {
                all_zeroes = false;
            }
        }
        if (all_zeroes) {
            break;
        }
    }
    for (int64_t i = xs_count - 1; i > 0; i--) {
        xs[i - 1] -= xs[i];
    }
    return xs[0];
}

int main(void)
{
    int64_t total_a = 0;
    int64_t total_b = 0;

    char line[1024];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t line_len = strnlen(line, sizeof(line));
        line[--line_len] = '\0';
        if (line_len == 0) {
            continue;
        }

        int64_t *xs_a = NULL, xs_count = 0;
        numbers_parse(line, &xs_a, &xs_count);

        int64_t* xs_b = malloc(xs_count * sizeof(xs_b[0]));
        memcpy(xs_b, xs_a, xs_count * sizeof(xs_a[0]));

        int64_t resp_a = extrapolate(xs_a, xs_count);
        total_a += resp_a;

        int64_t resp_b = extrapolate_backwards(xs_b, xs_count);
        total_b += resp_b;

        free(xs_b);
        free(xs_a);
    }
    printf("Part A: %ld\n", total_a);
    printf("Part B: %ld\n", total_b);
}

const char* numbers_parse(const char* s, int64_t** xs, int64_t* len)
{
    assert(s != NULL);
    assert(xs != NULL);
    assert(len != NULL);

    *xs = NULL;
    *len = 0;
    int64_t cap = 0;

    const char* cursor = s;
    while (true) {
        char* end = NULL;
        int64_t x = strtoll(cursor, &end, 10);
        assert(errno == 0);

        // end of input
        if (end == cursor) {
            break;
        }

        // append
        if (*len >= cap) {
            cap = (cap == 0) ? 8 : (cap * 2);
            *xs = realloc(*xs, cap * sizeof(**xs));
            assert(*xs != NULL);
        }
        (*xs)[*len] = x;
        *len += 1;

        cursor = end;
    }

    return cursor;
}

