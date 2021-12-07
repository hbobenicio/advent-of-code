#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include <sys/types.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#ifndef MAX_LINE_LEN
#define MAX_LINE_LEN 16
#endif

struct input {
    char lines[1024][MAX_LINE_LEN];
    size_t lines_count;
    size_t line_len;
};

static size_t solve(struct input* input);
static size_t oxygen_generator_rating(struct input* input);
static size_t co2_scrubber_rating(struct input* input);

static char most_common_value(char** lines, size_t index, char tie_value);
static char least_common_value(char** lines, size_t index, char tie_value);

int main()
{
    struct input input = {0};
    while (scanf("%s", input.lines[input.lines_count]) == 1) {
        input.lines_count++;
    }
    input.line_len = strnlen(input.lines[0], MAX_LINE_LEN);
    printf("%zu\n", solve(&input));
}

static size_t solve(struct input* input)
{
    size_t o2_rating = oxygen_generator_rating(input);
    fprintf(stderr, "o2: %zu\n", o2_rating);
    size_t co2_rating = co2_scrubber_rating(input);
    fprintf(stderr, "co2: %zu\n", co2_rating);
    return o2_rating * co2_rating;
}

static size_t oxygen_generator_rating(struct input* input)
{
    char tie_value = '1';
    char** selections = NULL;
    for (size_t i = 0; i < input->lines_count; i++) {
        arrpush(selections, input->lines[i]);
    }
    size_t index = 0;
    while (arrlen(selections) > 1 && index < input->line_len) {
        char val = most_common_value(selections, index, tie_value);

        // NOTE this mess could probably be improved...
        char** aux = NULL;
        arrsetcap(aux, arrcap(selections - 1));
        for (long int i = 0; i < arrlen(selections); i++) {
            char* s = selections[i];
            if (s[index] == val) {
                arrpush(aux, s);
            }
        }
        arrsetlen(selections, 0);
        for (long int i = 0; i < arrlen(aux); i++) {
            arrpush(selections, aux[i]);
        }
        arrfree(aux);
        index++;
    }
    assert(arrlen(selections) == 1);
    char* num = selections[0];
    fprintf(stderr, "o2: %s\n", num);
    
    size_t rating = 0;
    for (ssize_t i = input->line_len - 1, power = 0; i >= 0; i--, power++) {
        if (num[i] == '1') {
            rating += pow(2.0, power);
        }
    }
    arrfree(selections);
    return rating;
}

static size_t co2_scrubber_rating(struct input* input)
{
    char tie_value = '0';
    char** selections = NULL;
    for (size_t i = 0; i < input->lines_count; i++) {
        arrpush(selections, input->lines[i]);
    }
    size_t index = 0;
    while (arrlen(selections) > 1 && index < input->line_len) {
        char val = least_common_value(selections, index, tie_value);

        // NOTE this mess could probably be improved...
        char** aux = NULL;
        arrsetcap(aux, arrcap(selections - 1));
        for (long int i = 0; i < arrlen(selections); i++) {
            char* s = selections[i];
            if (s[index] == val) {
                arrpush(aux, s);
            }
        }
        arrsetlen(selections, 0);
        for (long int i = 0; i < arrlen(aux); i++) {
            arrpush(selections, aux[i]);
        }
        arrfree(aux);

        index++;
    }
    assert(arrlen(selections) == 1);
    char* num = selections[0];
    fprintf(stderr, "co2: %s\n", num);
    
    size_t rating = 0;
    for (ssize_t i = input->line_len - 1, power = 0; i >= 0; i--, power++) {
        if (num[i] == '1') {
            rating += pow(2.0, power);
        }
    }
    arrfree(selections);
    return rating;
}

static char most_common_value(char** lines, size_t index, char tie_value)
{
    long int bit_count = 0;
    long int lines_len = arrlen(lines);
    for (long int i = 0; i < lines_len; i++) {
        char* line = lines[i];
        if (line[index] == '1') {
            bit_count++;
        }
    }

    if (lines_len % 2 == 0) {
        if (bit_count == lines_len / 2) {
            return tie_value;
        }
    }
    return (bit_count > lines_len / 2) ? '1' : '0';
}

static char least_common_value(char** lines, size_t index, char tie_value)
{
    long int bit_count = 0;
    long int lines_len = arrlen(lines);
    for (long int i = 0; i < lines_len; i++) {
        char* line = lines[i];
        if (line[index] == '1') {
            bit_count++;
        }
    }

    if (lines_len % 2 == 0) {
        if (bit_count == lines_len / 2) {
            return tie_value;
        }
    }
    return (bit_count > lines_len / 2) ? '0' : '1';
}
