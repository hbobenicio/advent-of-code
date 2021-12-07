#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

typedef enum {
    ROCK,
    PAPER,
    SCISSORS,
} Shape;

typedef enum {
    LOSE,
    DRAW,
    WIN,
} Outcome;

Shape parse_opponent_shape(char c) {
    switch (c) {
        case 'A': return ROCK;
        case 'B': return PAPER;
        case 'C': return SCISSORS;
    }
    fprintf(stderr, "error: unknown shape '%c'\n", c);
    assert(false && "unreachable");
}

Outcome parse_my_outcome(char c) {
    switch (c) {
        case 'X': return LOSE;
        case 'Y': return DRAW;
        case 'Z': return WIN;
    }
    fprintf(stderr, "error: unknown outcome '%c'\n", c);
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

size_t outcome_score(Outcome my_outcome)
{
    if (my_outcome == DRAW) return 3;
    if (my_outcome == WIN) return 6;
    return 0;
}

Shape my_shape(Shape opponent_shape, Outcome my_outcome)
{
    if (my_outcome == DRAW) return opponent_shape;

    if (my_outcome == WIN) {
        if (opponent_shape == ROCK) return PAPER;
        if (opponent_shape == PAPER) return SCISSORS;
        assert(opponent_shape == SCISSORS);
        return ROCK;
    }

    assert(my_outcome == LOSE);
    if (opponent_shape == ROCK) return SCISSORS;
    if (opponent_shape == PAPER) return ROCK;
    assert(opponent_shape == SCISSORS);
    return PAPER;
}

size_t score(Shape opponent_shape, Outcome my_outcome)
{
    return shape_score(my_shape(opponent_shape, my_outcome)) + outcome_score(my_outcome);
}

int main()
{
    char opponent, mine;
    size_t total_score = 0;

    while (scanf("%c %c", &opponent, &mine) == 2)
    {
        Shape opponent_shape = parse_opponent_shape(opponent);
        Outcome my_outcome = parse_my_outcome(mine);

        total_score += score(opponent_shape, my_outcome);

        // skip trailling '\n'
        if(!feof(stdin)) getchar();
    }

    printf("%zu\n", total_score);
}

