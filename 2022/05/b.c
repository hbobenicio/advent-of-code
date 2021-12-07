#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#include <sys/types.h>
#include <unistd.h>

#define STACK_CAPACITY 512

typedef struct {
    char items[STACK_CAPACITY];
    size_t items_count;
} Stack;

typedef struct {
    size_t quantity, from, to;
} Move;

void stack_push(Stack* s, char c) {
    assert(s->items_count < STACK_CAPACITY);
    s->items[s->items_count++] = c;
}

char stack_pop(Stack* s) {
    assert(s->items_count > 0);
    return s->items[--s->items_count];
}

void stack_move(Stack* from, Stack* to, size_t crates_count) {
    Stack aux = {0};
    for (size_t i = 0; i < crates_count; i++) {
        stack_push(&aux, stack_pop(from));
    }
    for (size_t i = 0; i < crates_count; i++) {
        stack_push(to, stack_pop(&aux));
    }
}

char stack_peek_top(Stack* s) {
    return (s->items_count == 0)
         ? ' '
         : s->items[s->items_count - 1]
         ;
}

bool parse_move(Move* m) {
    return scanf("move %zu from %zu to %zu", &m->quantity, &m->from, &m->to) == 3;
}

int main() {
    // [F]         [L]     [M]
    // [T]     [H] [V] [G] [V]
    // [N]     [T] [D] [R] [N]     [D]
    // [Z]     [B] [C] [P] [B] [R] [Z]
    // [M]     [J] [N] [M] [F] [M] [V] [H]
    // [G] [J] [L] [J] [S] [C] [G] [M] [F]
    // [H] [W] [V] [P] [W] [H] [H] [N] [N]
    // [J] [V] [G] [B] [F] [G] [D] [H] [G]
    //  1   2   3   4   5   6   7   8   9
    Stack stacks[10] = {0};

    char lines[8][128] = {0};
    for (size_t i = 0; i < 8; i++) {
        assert(fgets(lines[i], 128, stdin) != NULL);
    }
    for (ssize_t i = 8 - 1; i >= 0; i--) {
        if (lines[i][ 1] != ' ') { printf("pushing %c to stack 1\n", lines[i][ 1]); stack_push(&stacks[0], lines[i][ 1]); } 
        if (lines[i][ 5] != ' ') { printf("pushing %c to stack 2\n", lines[i][ 5]); stack_push(&stacks[1], lines[i][ 5]); }
        if (lines[i][ 9] != ' ') { printf("pushing %c to stack 3\n", lines[i][ 9]); stack_push(&stacks[2], lines[i][ 9]); }
        if (lines[i][13] != ' ') { printf("pushing %c to stack 4\n", lines[i][13]); stack_push(&stacks[3], lines[i][13]); }
        if (lines[i][17] != ' ') { printf("pushing %c to stack 5\n", lines[i][17]); stack_push(&stacks[4], lines[i][17]); }
        if (lines[i][21] != ' ') { printf("pushing %c to stack 6\n", lines[i][21]); stack_push(&stacks[5], lines[i][21]); }
        if (lines[i][25] != ' ') { printf("pushing %c to stack 7\n", lines[i][25]); stack_push(&stacks[6], lines[i][25]); }
        if (lines[i][29] != ' ') { printf("pushing %c to stack 8\n", lines[i][29]); stack_push(&stacks[7], lines[i][29]); }
        if (lines[i][33] != ' ') { printf("pushing %c to stack 9\n", lines[i][33]); stack_push(&stacks[8], lines[i][33]); }
    }
    // skip 2 lines
    fgets(lines[0], 128, stdin);
    fgets(lines[0], 128, stdin);

    Move move;
    while (parse_move(&move)) {
        printf("move %zu from %zu to %zu\n", move.quantity, move.from, move.to);
        //while (move.quantity--) {
            //char c = stack_pop(&stacks[move.from - 1]);
            //stack_push(&stacks[move.to - 1], c);
        //}
        stack_move(&stacks[move.from - 1], &stacks[move.to - 1], move.quantity);
        if (!feof(stdin)) getchar();
    }
    for (size_t i = 0; i < 10; i++) {
        putchar(stack_peek_top(&stacks[i]));
    }
    putchar('\n');
}

