#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

int cmp_desc(const void* a, const void* b) {
    size_t x = *((const size_t*) a);
    size_t y = *((const size_t*) b);
    return y - x;
}

int main() {
    size_t elves_calories[1024] = {0};
    size_t elves_count = 0;
    char line[64] = {0};
    size_t sum = 0;
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t calory = 0;
        if (strlen(line) == 1) {
            elves_calories[elves_count++] = sum;
            sum = 0;
            continue;
        }
        assert(sscanf(line, "%zu", &calory) == 1);
        sum += calory;
    }

    qsort(elves_calories, elves_count, sizeof(elves_calories[0]), cmp_desc);

    printf("%zu\n", elves_calories[0]);
    printf("%zu\n", elves_calories[1]);
    printf("%zu\n", elves_calories[2]);
    printf("total: %zu\n", elves_calories[0] + elves_calories[1] + elves_calories[2]);
}

