#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

typedef enum {
    ROCK,
    PAPER,
    SCISSORS,
} Shape;

Shape parse_opponent_shape(char c) {
    switch (c) {
        case 'A': return ROCK;
        case 'B': return PAPER;
        case 'C': return SCISSORS;
    }
    fprintf(stderr, "error: unknown shape '%c'\n", c);
    assert(false && "unreachable");
}

Shape parse_my_shape(char c) {
    switch (c) {
        case 'X': return ROCK;
        case 'Y': return PAPER;
        case 'Z': return SCISSORS;
    }
    fprintf(stderr, "error: unknown shape '%c'\n", c);
    assert(false && "unreachable");
}

size_t shape_score(Shape shape)
{
    switch (shape) {
        case ROCK: return 1;
        case PAPER: return 2;
        case SCISSORS: return 3;
    }
    assert(false && "unreachable");
}

size_t outcome_score(Shape opponent_shape, Shape my_shape)
{
    // we draw
    if (opponent_shape == my_shape) return 3;

    // we win
    if (opponent_shape == ROCK && my_shape == PAPER) return 6;
    if (opponent_shape == PAPER && my_shape == SCISSORS) return 6;
    if (opponent_shape == SCISSORS && my_shape == ROCK) return 6;

    // we lose
    return 0;
}

size_t score(Shape opponent_shape, Shape my_shape)
{
    return shape_score(my_shape) + outcome_score(opponent_shape, my_shape);
}

int main()
{
    char opponent, mine;
    size_t total_score = 0;

    while (scanf("%c %c", &opponent, &mine) == 2)
    {
        Shape opponent_shape = parse_opponent_shape(opponent);
        Shape my_shape = parse_my_shape(mine);

        total_score += score(opponent_shape, my_shape);

        // skip trailling '\n'
        if(!feof(stdin)) getchar();
    }

    printf("%zu\n", total_score);
}

