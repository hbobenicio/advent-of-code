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

typedef struct {
    Coord* values;
    size_t count;
    size_t capacity;
} Coords;

void coords_free(Coords* coords)
{
    free(coords->values);
    coords->values = NULL;
    coords->capacity = coords->count = 0;
}

void coords_append(Coords* coords, Coord value)
{
    if (coords->count >= coords->capacity) {
        coords->capacity = (coords->capacity == 0) ? 8 : coords->capacity * 2;
        coords->values = realloc(coords->values, coords->capacity * sizeof(coords->values[0]));
    }
    coords->values[coords->count] = value;
    coords->count++;
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

Coords traverse(Input* input, Direction from, Coord pos)
{
    Coords loop_coords = {0};
    coords_append(&loop_coords, pos);

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
        coords_append(&loop_coords, pos);
    }

    return loop_coords;
}

/**
 * Performs a linear search O(n) on the coords of the loop and check if it has the given row/col.
 */
bool loop_coords_contains(const Coords* loop_coords, ssize_t row, ssize_t col)
{
    for (size_t i = 0; i < loop_coords->count; i++) {
        const Coord* coord = &loop_coords->values[i];
        if (coord->row == row && coord->col == col) {
            return true;
        }
    }
    return false;
}

/**
 * Counts the amount of positions inside the loop.
 * @deprecated can't handle borders (and probably impossible if considering only 1 direction)
 */
size_t broken_count_insiders(const Input* input, const Coords* loop_coords)
{
    size_t inside_count = 0;

    enum {
        OUTSIDE,
        ENTERING_LOOP,
        INSIDE,
        EXITING_LOOP,
    } state;

    for (size_t i = 0; i < input->rows; i++) {
        state = OUTSIDE;
        for (size_t j = 0; j < input->cols; j++) {
            bool contains = loop_coords_contains(loop_coords, i, j);

            if (state == OUTSIDE && !contains) {
                putchar('O');
                continue;
            }
            if (state == OUTSIDE && contains) {
                state = ENTERING_LOOP;
                putchar('P');
                continue;
            }
            if (state == ENTERING_LOOP && !contains) {
                state = INSIDE;
                inside_count++;
                putchar('I');
                continue;
            }
            if (state == ENTERING_LOOP && contains) {
                state = EXITING_LOOP;
                putchar('P');
                continue;
            }
            if (state == INSIDE && !contains) {
                inside_count++;
                putchar('I');
                continue;
            }
            if (state == INSIDE && contains) {
                state = EXITING_LOOP;
                putchar('P');
                continue;
            }
            if (state == EXITING_LOOP && !contains) {
                state = OUTSIDE;
                putchar('O');
                continue;
            }
            if (state == EXITING_LOOP && contains) {
                state = ENTERING_LOOP;
                putchar('P');
                continue;
            }
            assert(false); // all cases should be handled
        }
        putchar('\n');
    }
    return inside_count;
}

#define define_is_inside(Dir) \
bool is_inside_##Dir(const Input* input, const Coords* loop_coords, Coord c) \
{ \
    size_t cross_count = 0; \
    for (Coord x = coord_get_##Dir(c); is_inbound(input, x); x = coord_get_##Dir(x)) { \
        if (loop_coords_contains(loop_coords, x.row, x.col)) { \
            cross_count++; \
        } \
    } \
    return cross_count % 2 == 1; \
}

define_is_inside(north)
define_is_inside(east)
define_is_inside(south)
define_is_inside(west)

/**
 * Counts the amount of positions inside the loop.
 * @deprecated can't handle 'turns'
 */
size_t broken_count_insiders_2(const Input* input, const Coords* loop_coords)
{
    size_t insiders_count = 0;

    // Algorithm:
    // - find the bouding box (save min/max of row/col each)
    // - for each tile in the bounding box
    for (size_t i = 0; i < input->rows; i++) {
        for (size_t j = 0; j < input->cols; j++) {
            Coord c = { .row = i, .col = j };

            /*
            if (i == 4 && j == 7) {
                printf("Tile[%zu][%zu]: %c Border=%d N=%d E=%d S=%d W=%d\n", i, j, input_tile_get(input, c),
                       loop_coords_contains(loop_coords, c.row, c.col),
                       is_inside_north(input, loop_coords, c),
                       is_inside_east(input, loop_coords, c),
                       is_inside_south(input, loop_coords, c),
                       is_inside_west(input, loop_coords, c)
                );
            }
            */
            
            // - check if it's inside the loop:
            //   - for each direction, go until finding a bouding box border or going outbounds
            //   - if hit the border, go next. if it outbounds, the tile is outbounds
            if (loop_coords_contains(loop_coords, c.row, c.col)) {
                continue;
            }
            
            if (is_inside_north(input, loop_coords, c) && is_inside_south(input, loop_coords, c)) {
                printf("Inside: "); coord_print(c); printf("\n");
                insiders_count++;
                continue;
            }
            if (is_inside_east(input, loop_coords, c) && is_inside_west(input, loop_coords, c)) {
                printf("Inside: "); coord_print(c); printf("\n");
                insiders_count++;
                continue;
            }
           
            /*
            if (is_inside_north(input, loop_coords, c)) continue;
            if (is_inside_east(input, loop_coords, c)) continue;
            if (is_inside_south(input, loop_coords, c)) continue;
            if (is_inside_west(input, loop_coords, c)) continue;
            */

            //printf("Inside: "); coord_print(c); printf("\n");
            //insiders_count++;
        }
    }

    return insiders_count;
} 

size_t solve(Input* input)
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

        Coords loop_coords = traverse(input, from, next_coord);

        printf("Loop length: %zu\n", loop_coords.count);
        printf("Part A: %zu\n", loop_coords.count / 2);

        //size_t insiders_count = broken_count_insiders(input, &loop_coords);
        //coords_free(&loop_coords);
        //return insiders_count;

        size_t insiders_count = broken_count_insiders_2(input, &loop_coords);
        
        coords_free(&loop_coords);
        return insiders_count;
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

    printf("Part B: %zu\n", solve(&input));

    input_free(&input);
}

