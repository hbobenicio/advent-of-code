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

char solve(const char* rucksack, size_t rucksack_len)
{
    assert(rucksack_len % 2 == 0);
    size_t mid = rucksack_len / 2;
    for (size_t i = 0; i < mid; i++) {
        for (size_t j = mid; j < rucksack_len; j++) {
            if (rucksack[i] == rucksack[j]) {
                return rucksack[i];
            }
        }
    }
    assert(false && "unreachable: no duplicate item found");
}

int main()
{
    size_t total = 0;
    char line[128] = {0};
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t line_len = strlen(line);
        line[line_len - 1] = '\0';
        line_len--;

        char c = solve(line, line_len);
        total += priority(c);
    }
    printf("%zu\n", total);
}

