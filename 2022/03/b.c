//strnlen
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>

size_t priority(char c)
{
    assert(isupper(c) || islower(c));

    if (islower(c)) return c - 96;

    assert(isupper(c));
    return c - 38;
}

char solve(char rucksacks[3][128], size_t rucksacks_len[3])
{
    for (size_t i = 0; i < rucksacks_len[0]; i++) {
        for (size_t j = 0; j < rucksacks_len[1]; j++) {
            for (size_t k = 0; k < rucksacks_len[2]; k++) {
                if (rucksacks[0][i] == rucksacks[1][j] && rucksacks[1][j] == rucksacks[2][k]) {
                    return rucksacks[0][i];
                }
            }
        }
    }
    assert(false && "unreachable: no item found");
}

size_t read_rucksack(char line[128])
{
    if (fgets(line, 128, stdin) == NULL) return 0;
    size_t line_len = strnlen(line, 128);
    line[line_len - 1] = '\0';
    line_len--;
    return line_len;
}

int main()
{
    size_t total = 0;
    char lines[3][128] = {0};
    size_t lines_len[3];
    while(!feof(stdin)) {
        for (size_t i = 0; i < 3; i++) {
            lines_len[i] = read_rucksack(lines[i]);
            if (lines_len[i] == 0) goto exit;
        }
        char c = solve(lines, lines_len);
        total += priority(c);
    }
exit:
    printf("%zu\n", total);
}

