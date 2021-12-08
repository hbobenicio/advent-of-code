#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#ifndef INPUT_BUF_CAP
#define INPUT_BUF_CAP 2048
#endif

static size_t parse_input(int* out_buf, size_t out_buf_len)
{
    int x;
    size_t i = 0;
    while (scanf("%d", &x) == 1) {
        out_buf[i++] = x;
        assert((i < out_buf_len) && "input buffer overflow");
    }
    return i;
}

static size_t solve(int* xs, size_t xs_len)
{
    size_t increase_count = 0;
    int prev = INT_MAX;
    for (size_t i = 0; i < xs_len; prev = xs[i++]) {
        if (prev < xs[i]) {
            increase_count++;
        }
    }
    return increase_count;
}

int main() {
    int xs[INPUT_BUF_CAP] = {0};
    size_t xs_len = parse_input(xs, sizeof(xs) / sizeof(xs[0]));
    printf("%zu\n", solve(xs, xs_len));
}

