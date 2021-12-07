#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

size_t numlen(size_t x) {
    if (x < 10) return 1;
    return 1 + numlen(x / 10);
}

size_t A(size_t* winning_numbers, size_t winning_numbers_count, size_t* card_numbers, size_t card_numbers_count)
{
    size_t matches_count = 0;
    for (size_t i = 0; i < winning_numbers_count; i++) {
        for (size_t j = 0; j < card_numbers_count; j++) {
            if (winning_numbers[i] == card_numbers[j]) {
                matches_count++;
                break;
            }
        }
    }
    if (matches_count == 0) {
        return 0;
    }
    size_t score = (size_t) pow(2, matches_count - 1);
    return score;
}

int main(void)
{
    size_t solution_a = 0;

    char line[512];
    size_t lines_count = 0;
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t line_len = strnlen(line, sizeof(line));
        assert(line_len > 0);
        line[--line_len] = '\0';
        if (line_len == 0) {
            continue;
        }
        lines_count++;

        size_t card_id;
        size_t *winning_numbers = NULL, winning_numbers_count = 0;
        size_t *card_numbers = NULL, card_numbers_count = 0;
        const char* cursor = line;

        int rc = sscanf(cursor, "Card %zu", &card_id);
        assert(rc == 1);
        cursor = strchr(cursor, ':') + 2;
        while (isspace(*cursor)) {
            cursor++;
        }

        size_t x;
        while (sscanf(cursor, "%zu", &x) == 1) {
            winning_numbers_count++;
            winning_numbers = realloc(winning_numbers, winning_numbers_count * sizeof(winning_numbers[0]));
            winning_numbers[winning_numbers_count - 1] = x;
            cursor += numlen(x);
            while (isspace(*cursor)) {
                cursor++;
            }
        }

        cursor = strchr(cursor, '|') + 2;

        while (sscanf(cursor, "%zu", &x) == 1) {
            card_numbers_count++;
            card_numbers = realloc(card_numbers, card_numbers_count * sizeof(card_numbers[0]));
            card_numbers[card_numbers_count - 1] = x;
            cursor += numlen(x);
            while (isspace(*cursor)) {
                cursor++;
            }
        }

        size_t card_points = A(winning_numbers, winning_numbers_count, card_numbers, card_numbers_count);
        solution_a += card_points;

        
        free(card_numbers);
        free(winning_numbers);
   }

   printf("A: %zu\n", solution_a);
}

