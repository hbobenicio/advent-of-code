/**
 * This is a total mess, but it worked.
 * I have no time nor desire to refactor this or make this pretty.
 * Anyway, there it is... as-is.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    char** items;
    size_t rows;
    size_t cols;
} Grid;

typedef struct {
    size_t num;
    size_t row;
    size_t col_start;
    size_t col_end;
} NumberOccurrence;

void grid_free(Grid* grid);
void grid_rows_append(Grid* grid, const char* line, size_t line_len);
bool grid_cell_has_adjacent_symbol(Grid* grid, size_t row, size_t col);
void grid_print(Grid* grid);
bool is_symbol(int c);
bool is_gear(Grid* grid, size_t row, size_t col);
bool is_digit_next_to_gear(Grid* grid, size_t row, size_t col);

size_t solve_part1(Grid* grid)
{
    NumberOccurrence* num_occurrences = NULL;
    size_t num_occurrences_size = 0;
    int state = 0; // 0: parsing not digit, 1: parsing digit
    size_t sum = 0;

    for (size_t row = 0; row < grid->rows; row++) {
        for (size_t col = 0; col < grid->cols; col++) {
            char c = grid->items[row][col];
            if (!isdigit(c)) {
                if (state == 1) {
                    NumberOccurrence* no = &num_occurrences[num_occurrences_size - 1];
                    no->col_end = col - 1;

                    char num_buf[9] = {0};
                    memcpy(num_buf, &grid->items[row][no->col_start], no->col_end - no->col_start + 1);
                    no->num = (size_t) atoi(num_buf);
                    state = 0;
                }
                continue;
            }

            if (state == 0) {
                // add room to a new number
                num_occurrences_size++;
                num_occurrences = realloc(num_occurrences, num_occurrences_size * sizeof(num_occurrences[0]));

                num_occurrences[num_occurrences_size - 1].row = row;
                num_occurrences[num_occurrences_size - 1].col_start = col;
                state = 1;
            }
        }
        if (state == 1) {
            NumberOccurrence* no = &num_occurrences[num_occurrences_size - 1];
            no->col_end = grid->cols - 1;

            char num_buf[9] = {0};
            memcpy(num_buf, &grid->items[row][no->col_start], no->col_end - no->col_start + 1);
            no->num = (size_t) atoi(num_buf);
            state = 0;
        }
    }

    for (size_t i = 0; i < num_occurrences_size; i++) {
        NumberOccurrence* no = &num_occurrences[i];
        for (size_t col = no->col_start; col <= no->col_end; col++) {
            if (grid_cell_has_adjacent_symbol(grid, no->row, col)) {
                sum += no->num;
                goto end_outer_loop;
            }
        }

    end_outer_loop:
        (void) i;
    }

    free(num_occurrences);

    return sum;
}

size_t solve_part2(Grid* grid)
{
    NumberOccurrence* num_occurrences = NULL;
    size_t num_occurrences_size = 0;
    int state = 0; // 0: parsing not digit, 1: parsing digit

    for (size_t row = 0; row < grid->rows; row++) {
        for (size_t col = 0; col < grid->cols; col++) {
            char c = grid->items[row][col];
            if (!isdigit(c)) {
                if (state == 1) {
                    NumberOccurrence* no = &num_occurrences[num_occurrences_size - 1];
                    no->col_end = col - 1;

                    char num_buf[9] = {0};
                    memcpy(num_buf, &grid->items[row][no->col_start], no->col_end - no->col_start + 1);
                    no->num = (size_t) atoi(num_buf);
                    state = 0;
                }
                continue;
            }

            if (state == 0) {
                // add room to a new number
                num_occurrences_size++;
                num_occurrences = realloc(num_occurrences, num_occurrences_size * sizeof(num_occurrences[0]));

                num_occurrences[num_occurrences_size - 1].row = row;
                num_occurrences[num_occurrences_size - 1].col_start = col;
                state = 1;
            }
        }
        if (state == 1) {
            NumberOccurrence* no = &num_occurrences[num_occurrences_size - 1];
            no->col_end = grid->cols - 1;

            char num_buf[9] = {0};
            memcpy(num_buf, &grid->items[row][no->col_start], no->col_end - no->col_start + 1);
            no->num = (size_t) atoi(num_buf);
            state = 0;
        }
    }

    // - make a grid of gear rates initially zeroed out
    size_t* gear_rates = calloc(grid->rows * grid->cols, sizeof(size_t));

    // - traverse number occurrences and compute gear rates (I can only tell if the number needs to be included in the
    //   product after traversing all numbers from all gears
    for (size_t i = 0; i < num_occurrences_size; i++) {
        NumberOccurrence* no = &num_occurrences[i];
        for (size_t col = no->col_start; col <= no->col_end; col++) {
            for (ssize_t i = -1; i <= 1; i++) {
                for (ssize_t j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;

                    ssize_t ii = no->row + i;
                    if (ii < 0 || ii >= (ssize_t) grid->rows) continue;

                    ssize_t jj = col + j;
                    if (jj < 0 || jj >= (ssize_t) grid->cols) continue;

                    if (grid->items[ii][jj] == '*') {
                        gear_rates[ii * grid->cols + jj]++;
                        goto next_number;
                    }
                }
            }
        }
    next_number:
        (void) i;
    }

    // - traverse number occurrences again and check if any digit has neighbor of a gear with rate 2.
    //   compute the product of the numbers on the gear. store it on the prods sparse matrix
    size_t* prods = malloc(grid->rows * grid->cols * sizeof(*prods));
    for (size_t i = 0; i < grid->rows; i++) {
        for (size_t j = 0; j < grid->cols; j++) {
            prods[i * grid->cols + j] = 1;
        }
    }
    for (size_t i = 0; i < num_occurrences_size; i++) {
        NumberOccurrence* no = &num_occurrences[i];
        for (size_t col = no->col_start; col <= no->col_end; col++) {
            for (ssize_t i = -1; i <= 1; i++) {
                for (ssize_t j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;

                    ssize_t ii = no->row + i;
                    if (ii < 0 || ii >= (ssize_t) grid->rows) continue;

                    ssize_t jj = col + j;
                    if (jj < 0 || jj >= (ssize_t) grid->cols) continue;

                    if (grid->items[ii][jj] == '*' && gear_rates[ii * grid->cols + jj] == 2) {
                        prods[ii * grid->cols + jj] *= no->num;
                        goto next_number_2;
                    }
                }
            }
        }
    next_number_2:
        (void) i;
    }

    size_t result = 0;
    for (size_t i = 0; i < grid->rows; i++) {
        for (size_t j = 0; j < grid->cols; j++) {
            size_t prod = prods[i * grid->cols + j];
            if (prod != 1) {
                result += prod;
            }
        }
    }
 
    free(prods);
    free(gear_rates);
    free(num_occurrences);
    return result;
}

int main(void)
{
    Grid grid = {0};

    char line[512];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t line_len = strnlen(line, sizeof(line));
        assert(line_len > 0);
        line[--line_len] = '\0';
        
        // just skip empty lines from the input
        if (line_len == 0) {
            continue;
        }

        if (grid.cols == 0) {
            grid.cols = line_len;
        } else {
            // every line (aka row) must have the same size to be a grid
            assert(grid.cols == line_len);
        }

        grid_rows_append(&grid, line, line_len);
    }

    size_t solution = solve_part1(&grid);
    printf("Part 1 Solution: %zu\n", solution);

    solution = solve_part2(&grid);
    printf("Part 2 Solution: %zu\n", solution);

    grid_free(&grid);
}

void grid_free(Grid* grid)
{
    assert(grid != NULL);
    for (size_t i = 0; i < grid->rows; i++) {
        free(grid->items[i]);
    }
    free(grid->items);
}

void grid_rows_append(Grid* grid, const char* line, size_t line_len)
{
    assert(grid != NULL);
    assert(line != NULL);
    assert(line_len > 0);

    char* row = strndup(line, line_len);
    assert(row != NULL);

    grid->rows++;
    grid->items = realloc(grid->items, grid->rows * sizeof(grid->items[0]));
    grid->items[grid->rows - 1] = row;
}

void grid_print(Grid* grid)
{
    printf("Grid { .rows = %zu, .cols: %zu }\n", grid->rows, grid->cols);
    for (size_t i = 0; i < grid->rows; i++) {
        for (size_t j = 0; j < grid->cols; j++) {
            putchar(grid->items[i][j]);
        }
        putchar('\n');
    }
}

//NOTE this could be greatly improved with dynamic programming...
bool grid_cell_has_adjacent_symbol(Grid* grid, size_t row, size_t col)
{
    for (ssize_t i = -1; i <= 1; i++) {
        for (ssize_t j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;

            ssize_t ii = row + i;
            if (ii < 0 || ii >= (ssize_t) grid->rows) continue;

            ssize_t jj = col + j;
            if (jj < 0 || jj >= (ssize_t) grid->cols) continue;

            if (is_symbol(grid->items[ii][jj])) {
                return true;
            }
        }
    }
    return false;
}

bool is_symbol(int c)
{
    return !isdigit(c) && c != '.';
}

bool is_gear(Grid* grid, size_t row, size_t col)
{
    if (grid->items[row][col] != '*') {
        return false;
    }
    size_t neighbors_nums_count = 0;
    for (ssize_t i = -1; i <= 1; i++) {
        for (ssize_t j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;

            ssize_t ii = row + i;
            if (ii < 0 || ii >= (ssize_t) grid->rows) continue;

            ssize_t jj = col + j;
            if (jj < 0 || jj >= (ssize_t) grid->cols) continue;

            if (isdigit(grid->items[ii][jj])) {
                printf("neighbor digit found at (%zu, %zu): %c\n", ii, jj, grid->items[ii][jj]);
                neighbors_nums_count++;
            }
        }
    }
    printf("cell(%zu, %zu) has %zu neighbor digits\n", row, col, neighbors_nums_count);
    return neighbors_nums_count == 2;
}

bool is_digit_next_to_gear(Grid* grid, size_t row, size_t col)
{
    assert(isdigit(grid->items[row][col]));
    for (ssize_t i = -1; i <= 1; i++) {
        for (ssize_t j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;

            ssize_t ii = row + i;
            if (ii < 0 || ii >= (ssize_t) grid->rows) continue;

            ssize_t jj = col + j;
            if (jj < 0 || jj >= (ssize_t) grid->cols) continue;

            if (is_gear(grid, ii, jj)) {
                printf("here\n");
                return true;
            }
        }
    }
    return false;
}

