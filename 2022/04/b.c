#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct {
    int x, y;
} Range;

bool range_fully_contains(Range r1, Range r2)
{
    return (r1.x <= r2.x && r2.y <= r1.y)
        || (r2.x <= r1.x && r1.y <= r2.y);
}

bool range_overlaps(Range r1, Range r2)
{
    //8-65,7-70
    return (r1.x <= r2.x && r2.x <= r1.y)
        || (r1.x <= r2.y && r2.y <= r1.y)
        || range_fully_contains(r1, r2);
}

int main() {
    Range r1, r2;
    size_t count = 0;
    while (scanf("%d-%d,%d-%d", &r1.x, &r1.y, &r2.x, &r2.y) == 4) {
        assert(r1.x <= r1.y);
        assert(r2.x <= r2.y);
        if (range_overlaps(r1, r2)) {
//            fprintf(stderr, "%d-%d,%d-%d overlaps\n", r1.x, r1.y, r2.x, r2.y);
            count++;
        } else {
            fprintf(stderr, "%d-%d,%d-%d DO NOT overlaps\n", r1.x, r1.y, r2.x, r2.y);
        }
    }
    printf("%zu\n", count);
}

