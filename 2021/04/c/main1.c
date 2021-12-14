#include <stdio.h>
#include <stdlib.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

struct board_cell {
    int number;
};

struct board {
    struct board_cell cells[5][5];
};

struct input {
    int* numbers;
    struct board* boards;
};

int main() {
    struct input input = {0};
    int x;
    scanf("%d,", &x);
    printf("%d\n", x);
    scanf("%d,", &x);
    printf("%d\n", x);
    scanf("%d,", &x);
    printf("%d\n", x);
}