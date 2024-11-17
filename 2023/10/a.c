#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

typedef struct {
    char** tiles;
    size_t rows;
    size_t cols;
    size_t cap;
} Input;

void input_tiles_append(Input* input, const char* row, size_t row_len)
{
    if (input->rows >= input->cap) {
        input->cap = (input->cap == 0) ? 8 : input->cap * 2;
        input->tiles = realloc(input->tiles, input->cap * sizeof(input->tiles[0]));
    }
    input->tiles[input->rows] = strdup(row);
    input->rows++;
    input->cols = row_len;
}

void input_free(Input* input)
{
    for (size_t i = 0; i < input->rows; i++) {
        free(input->tiles[i]);
        input->tiles[i] = NULL;
    }
    input->cap = input->rows = input->cols = 0;
}

typedef enum {
    NORTH, EAST, SOUTH, WEST
} Direction;

Direction direction_opposite(Direction d)
{
    switch (d) {
        case NORTH: return SOUTH;
        case EAST:  return WEST;
        case SOUTH: return NORTH;
        case WEST:  return EAST;
    }
    assert(false);
}

typedef struct {
    ssize_t row;
    ssize_t col;
} Coord;

Coord coord_get_north(Coord c) { return (Coord) { .row = c.row - 1, .col = c.col     }; }
Coord coord_get_east (Coord c) { return (Coord) { .row = c.row    , .col = c.col + 1 }; }
Coord coord_get_south(Coord c) { return (Coord) { .row = c.row + 1, .col = c.col     }; }
Coord coord_get_west (Coord c) { return (Coord) { .row = c.row    , .col = c.col - 1 }; }

void coord_print(Coord c)
{
    printf(" (%zu, %zu)", c.row, c.col);
}

/**
 * Finds the starting position.
 */
Coord find_s(const Input* input)
{
    for (size_t i = 0; i < input->rows; i++) {
        for (size_t j = 0; j < input->cols; j++) {
            if (input->tiles[i][j] == 'S') {
                return (Coord) { .row = i, .col = j };
            }
        }
    }
    assert(false); // there must be at least 1 starting position
}

bool is_inbound(const Input* input, Coord c)
{
    if (c.row < 0 || c.col < 0) return false;
    if (c.row >= (ssize_t) input->rows || c.col >= (ssize_t) input->cols) return false;
    return true;
}

char input_tile_get(const Input* input, Coord c)
{
    return input->tiles[c.row][c.col];
}

void traverse(Input* input, Direction from, Coord pos, size_t* distance)
{
    for (char c = input_tile_get(input, pos); c != 'S'; c = input_tile_get(input, pos)) {
        switch (from) {
            case NORTH: {
                if (c == '|') {
                    pos = coord_get_south(pos);
                } else if (c == 'J') {
                    pos = coord_get_west(pos);
                    from = EAST;
                } else if (c == 'L') {
                    pos = coord_get_east(pos);
                    from = WEST;
                } else {
                    assert(false); // nowhere to go
                }
            } break;

            case EAST: {
                if (c == '-') {
                    pos = coord_get_west(pos);
                } else if (c == 'L') {
                    pos = coord_get_north(pos);
                    from = SOUTH;
                } else if (c == 'F') {
                    pos = coord_get_south(pos);
                    from = NORTH;
                } else {
                    assert(false); // nowhere to go
                }
            } break;

            case SOUTH: {
                if (c == '|') {
                    pos = coord_get_north(pos);
                } else if (c == '7') {
                    pos = coord_get_west(pos);
                    from = EAST;
                } else if (c == 'F') {
                    pos = coord_get_east(pos);
                    from = WEST;
                } else {
                    assert(false); // nowhere to go
                }
            } break;

            case WEST: {
                if (c == '-') {
                    pos = coord_get_east(pos);
                } else if (c == '7') {
                    pos = coord_get_south(pos);
                    from = NORTH;
                } else if (c == 'J') {
                    pos = coord_get_north(pos);
                    from = SOUTH;
                } else {
                    assert(false); // nowhere to go
                }
            } break;
        }
        assert(is_inbound(input, pos));
        *distance += 1;
    }
}

size_t part_a(Input* input)
{
    // Find S
    Coord S = find_s(input);
    printf("S:"); coord_print(S); printf("\n");

    // Find S connecting neighbors
    Coord next_coord = {0};
    Direction from = {0};

    Coord north = coord_get_north(S);
    if (is_inbound(input, north)) {
        char tile = input_tile_get(input, north);
        if (tile == '|' || tile == '7' || tile == 'F') {
            next_coord = north;
            from = SOUTH;
            goto first_neighbor_selected;
        }
    }

    Coord east  = coord_get_east(S);
    if (is_inbound(input, east)) {
        char tile = input_tile_get(input, east);
        if (tile == '-' || tile == 'J' || tile == '7') {
            next_coord = east;
            from = WEST;
            goto first_neighbor_selected;
        }
    }

    Coord south = coord_get_south(S);
    if (is_inbound(input, south)) {
        char tile = input_tile_get(input, south);
        if (tile == '|' || tile == 'L' || tile == 'J') {
            next_coord = south;
            from = NORTH;
            goto first_neighbor_selected;
        }
    }

    Coord west = coord_get_west(S);
    if (is_inbound(input, west)) {
        char tile = input_tile_get(input, west);
        if (tile == '-' || tile == 'L' || tile == 'F') {
            next_coord = west;
            from = EAST;
            goto first_neighbor_selected;
        }
    }
first_neighbor_selected: {
        size_t distance = 1;
        traverse(input, from, next_coord, &distance);
        return distance / 2;
    }
}

int main(void)
{
    Input input = {0};

    char line[256];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t line_len = strnlen(line, sizeof(line));
        line[--line_len] = '\0';
        input_tiles_append(&input, line, line_len);
    }

    printf("Part A: %zu\n", part_a(&input));

    input_free(&input);
}

