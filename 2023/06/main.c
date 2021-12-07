#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <errno.h>

const char* parse_sizes(const char* s, size_t** xs, size_t* len);

typedef struct {
    size_t* values;
    size_t len;
} Numbers;

typedef struct {
    Numbers times;
    Numbers distances;
} Input;

#define S(Hold, Limit) ((Hold) * ((Limit) - (Hold)))

int main(void)
{
    Input input = {0};
    {
        char line[512];
        char* ptr = fgets(line, sizeof(line), stdin);
        assert(ptr != NULL);
        size_t line_len = strnlen(line, sizeof(line));
        assert(line_len > 0);
        line[--line_len] = '\0';
        parse_sizes(strchr(line, ':') + 2, &input.times.values, &input.times.len);
    }
    {
        char line[512];
        char* ptr = fgets(line, sizeof(line), stdin);
        assert(ptr != NULL);
        size_t line_len = strnlen(line, sizeof(line));
        assert(line_len > 0);
        line[--line_len] = '\0';
        parse_sizes(strchr(line, ':') + 2, &input.distances.values, &input.distances.len);
    }
    assert(input.times.len == input.distances.len);

    size_t total = 1;
    for (size_t r = 0; r < input.times.len; r++) {
        const size_t time_limit = input.times.values[r];
        const size_t record_distance = input.distances.values[r];
        size_t ways_count = 0;
        for (size_t t = 1; t < time_limit - 1; t++) {
            if (S(t, time_limit) > record_distance) {
                ways_count++;
            }
        }
        printf("ways of winning race %zu: %zu\n", r+1, ways_count);
        total *= ways_count;
    }
    printf("A: %zu\n", total);

    free(input.times.values);
    free(input.distances.values);
}

const char* parse_sizes(const char* s, size_t** xs, size_t* len)
{
    assert(s != NULL);
    assert(xs != NULL);
    assert(len != NULL);

    *xs = NULL;
    *len = 0;
    size_t cap = 0;

    const char* cursor = s;
    while (true) {
        char* end = NULL;
        size_t x = strtoull(cursor, &end, 10);
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

