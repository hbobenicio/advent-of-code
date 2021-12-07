#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

enum direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};
#define DIRECTION_COUNT 4

enum visibility {
    VISIBILITY_NOT_SET = 0,
    VISIBILITY_VISIBLE,
    VISIBILITY_NOT_VISIBLE,
};

struct tree {
    enum visibility visibility[DIRECTION_COUNT];
    int height;
};

struct forest {
    struct tree* trees;
    size_t rows_count;
    size_t cols_count;
};

void forest_free(struct forest* forest) {
    assert(forest != NULL);
    free(forest->trees);
    forest->trees = NULL;
    forest->rows_count = forest->cols_count = 0;
}

struct tree* forest_get(struct forest* forest, size_t row, size_t col) {
    assert(forest != NULL);
    assert(row < forest->rows_count);
    assert(col < forest->cols_count);

    return &forest->trees[row * forest->cols_count + col];
}

void forest_row_append(struct forest* forest) {
    assert(forest != NULL);

    forest->rows_count++;
    const size_t new_capacity = forest->rows_count * forest->cols_count * sizeof(*forest->trees);
    forest->trees = realloc(forest->trees, new_capacity);
    assert(forest->trees != NULL);
}

void tree_visibility_calc(struct forest* forest, struct tree* tree, size_t row, size_t col) {
    assert(tree != NULL);

    // edge: first row: always north visible
    if (row == 0) {
        tree->visibility[NORTH] = VISIBILITY_VISIBLE;
    }
    // edge: last row: always south visible
    if (row == forest->rows_count - 1) {
        tree->visibility[SOUTH] = VISIBILITY_VISIBLE;
    }
    // edge: first col: always west visible
    if (col == 0) {
        tree->visibility[WEST] = VISIBILITY_VISIBLE;
    }
    // edge: last col: always east visible
    if (col == forest->cols_count - 1) {
        tree->visibility[EAST] = VISIBILITY_VISIBLE;
    }

    // general case

    // north visibility calc
    if (row > 0) {
        size_t north_row = row - 1;
        struct tree* north_tree = forest_get(forest, north_row, col);
        assert(north_tree != NULL);

        if (north_tree->visibility[NORTH] == VISIBILITY_NOT_SET) {
            tree_visibility_calc(forest, north_tree, north_row, col);
        }
        assert(north_tree->visibility[NORTH] != VISIBILITY_NOT_SET);

        if (north_tree->visibility[NORTH] == VISIBILITY_VISIBLE && north_tree->height < tree->height) {
            tree->visibility[NORTH] = VISIBILITY_VISIBLE;
        } else {
            tree->visibility[NORTH] = VISIBILITY_NOT_VISIBLE;
        }
    }

    // south visibility calc
    if (row < forest->rows_count - 1) {
        size_t south_row = row + 1;
        struct tree* south_tree = forest_get(forest, south_row, col);
        assert(south_tree != NULL);

        if (south_tree->visibility[SOUTH] == VISIBILITY_NOT_SET) {
            tree_visibility_calc(forest, south_tree, south_row, col);
        }
        assert(south_tree->visibility[SOUTH] != VISIBILITY_NOT_SET);

        if (south_tree->visibility[SOUTH] == VISIBILITY_VISIBLE && south_tree->height < tree->height) {
            tree->visibility[SOUTH] = VISIBILITY_VISIBLE;
        } else {
            tree->visibility[SOUTH] = VISIBILITY_NOT_VISIBLE;
        }
    }

    // west visibility calc
    if (col > 0) {
        size_t west_col = col - 1;
        struct tree* west_tree = forest_get(forest, west_col, col);
        assert(west_tree != NULL);

        if (west_tree->visibility[WEST] == VISIBILITY_NOT_SET) {
            tree_visibility_calc(forest, west_tree, row, west_col);
        }
        assert(west_tree->visibility[WEST] != VISIBILITY_NOT_SET);

        if (west_tree->visibility[WEST] == VISIBILITY_VISIBLE && west_tree->height < tree->height) {
            tree->visibility[WEST] = VISIBILITY_VISIBLE;
        } else {
            tree->visibility[WEST] = VISIBILITY_NOT_VISIBLE;
        }
    }

    // east visibility calc
    if (col < forest->cols_count - 1) {
        size_t east_col = col + 1;
        struct tree* east_tree = forest_get(forest, east_col, col);
        assert(east_tree != NULL);

        if (east_tree->visibility[EAST] == VISIBILITY_NOT_SET) {
            tree_visibility_calc(forest, east_tree, row, east_col);
        }
        assert(east_tree->visibility[EAST] != VISIBILITY_NOT_SET);

        if (east_tree->visibility[EAST] == VISIBILITY_VISIBLE && east_tree->height < tree->height) {
            tree->visibility[EAST] = VISIBILITY_VISIBLE;
        } else {
            tree->visibility[EAST] = VISIBILITY_NOT_VISIBLE;
        }
    }
}

void forest_trees_visibility_calc(struct forest* forest) {
    assert(forest != NULL);
    for (size_t row = 0; row < forest->rows_count; row++) {
        for (size_t col = 0; col < forest->cols_count; col++) {
            struct tree* t = forest_get(forest, row, col);
            tree_visibility_calc(forest, t, row, col);
        }
    }
}

size_t forest_visibles_count(struct forest* forest) {
    assert(forest != NULL);
    size_t count = 0;
    for (size_t row = 0; row < forest->rows_count; row++) {
        for (size_t col = 0; col < forest->cols_count; col++) {
            struct tree* t = forest_get(forest, row, col);
            if (
                t->visibility[NORTH] == VISIBILITY_VISIBLE ||
                t->visibility[SOUTH] == VISIBILITY_VISIBLE ||
                t->visibility[WEST] == VISIBILITY_VISIBLE ||
                t->visibility[EAST] == VISIBILITY_VISIBLE
            ) {
                count++;
                fprintf(stderr, "debug: T(%zu, %zu)=%d is visible. count=%zu\n", row, col, t->height, count);
            } else {
                fprintf(stderr, "debug: T(%zu, %zu)=%d is not visible. visibility=[%d, %d, %d, %d]\n",
                        row, col,
                        t->height,
                        t->visibility[0],
                        t->visibility[1],
                        t->visibility[2],
                        t->visibility[3]
                );
            }
        }
    }
    return count;
}

int main(void)
{
    struct forest forest = {0};

    char line[256] = {0};
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t line_len = strlen(line);
        assert(line_len > 0);
        line[--line_len] = '\0';

        // if empty line, just skip it!
        if (line_len == 0) {
            continue;
        }

        if (forest.cols_count == 0) {
            forest.cols_count = line_len;
        }
        if (forest.cols_count != line_len) {
            fprintf(
                stderr,
                "error: forest cols count and line len mismatch: cols_count=%zu, line_len=%zu\n",
                forest.cols_count,
                line_len
            );
            goto err_forest_free;
        }

        forest_row_append(&forest);

        for (size_t col = 0; col < line_len; col++) {
            int h = line[col] - '0';
            printf("%d ", h);

            struct tree* t = forest_get(&forest, forest.rows_count - 1, col);
            t->height = h;
            t->visibility[NORTH] = VISIBILITY_NOT_SET;
            t->visibility[SOUTH] = VISIBILITY_NOT_SET;
            t->visibility[EAST]  = VISIBILITY_NOT_SET;
            t->visibility[WEST]  = VISIBILITY_NOT_SET;
        }
        printf("\n");
    }

    forest_trees_visibility_calc(&forest);
    printf("Solution A:\n");
    printf("  Total Visible Trees: %zu\n", forest_visibles_count(&forest));

    forest_free(&forest);
    return 0;

err_forest_free:
    forest_free(&forest);
    return 1;
}

