#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

typedef enum {
    PARSING_GAME_ID,
    PARSING_SET_START,
    PARSING_SET_FINISH,
    PARSING_DONE,
} ParsingState;

/**
 * Solution to part A of problem 02
 */
static bool is_game_possible(const char* line, size_t line_len, size_t* game_id)
{
    ParsingState state = PARSING_GAME_ID;
    const char* cursor = line;
    while (state != PARSING_DONE && cursor < line + line_len) {
        switch (state) {
        case PARSING_GAME_ID: {
            cursor += 5;
            int rc = sscanf(cursor, "%zu", game_id);
            assert(rc == 1);

            // skip the number until ':'
            while (*cursor != ':') {
                cursor++;
            }

            // skip the ': '
            cursor += 2;

            state = PARSING_SET_START;
        } break;
        case PARSING_SET_START: {
                size_t num;
                char color[16] = {0};
                int rc = sscanf(cursor, "%zu %s", &num, color);
                assert(rc == 2);
                size_t color_len = strlen(color);
                if (color[color_len - 1] == ',' || color[color_len - 1] == ';') {
                    color[--color_len] = '\0';
                }
                if (strcmp(color, "red") == 0 && num > 12) {
                    return false;
                }
                if (strcmp(color, "green") == 0 && num > 13) {
                    return false;
                }
                if (strcmp(color, "blue") == 0 && num > 14) {
                    return false;
                }
                while (cursor < line + line_len && *cursor != ',' && *cursor != ';') {
                    cursor++;
                }
                state = PARSING_SET_FINISH;
        } break;
        case PARSING_SET_FINISH: {
                if (cursor >= line + line_len) {
                    state = PARSING_DONE;
                    break;
                }
                if (*cursor == ',') {
                    cursor += 2;
                    state = PARSING_SET_START;
                    break;
                }
                if (*cursor == ';') {
                    cursor += 2;
                    state = PARSING_SET_START;
                    break;
                }
        } break;
        default:
            assert(false);
        }
    }
    return true;
}

/**
 * Solution to part B of problem 02
 */
static size_t compute_max(const char* line, size_t line_len, size_t* game_id)
{
    size_t max_red = 0;
    size_t max_green = 0;
    size_t max_blue = 0;

    ParsingState state = PARSING_GAME_ID;
    const char* cursor = line;
    while (state != PARSING_DONE && cursor < line + line_len) {
        switch (state) {
        case PARSING_GAME_ID: {
            cursor += 5;
            int rc = sscanf(cursor, "%zu", game_id);
            assert(rc == 1);

            // skip the number until ':'
            while (*cursor != ':') {
                cursor++;
            }

            // skip the ': '
            cursor += 2;

            state = PARSING_SET_START;
        } break;
        case PARSING_SET_START: {
                size_t num;
                char color[16] = {0};
                int rc = sscanf(cursor, "%zu %s", &num, color);
                assert(rc == 2);
                size_t color_len = strlen(color);
                if (color[color_len - 1] == ',' || color[color_len - 1] == ';') {
                    color[--color_len] = '\0';
                }
                if (strcmp(color, "red") == 0) {
                    max_red = MAX(max_red, num);
                } else if (strcmp(color, "green") == 0) {
                    max_green = MAX(max_green, num);
                } else if (strcmp(color, "blue") == 0) {
                    max_blue = MAX(max_blue, num);
                }
                while (cursor < line + line_len && *cursor != ',' && *cursor != ';') {
                    cursor++;
                }
                state = PARSING_SET_FINISH;
        } break;
        case PARSING_SET_FINISH: {
                if (cursor >= line + line_len) {
                    state = PARSING_DONE;
                    break;
                }
                if (*cursor == ',') {
                    cursor += 2;
                    state = PARSING_SET_START;
                    break;
                }
                if (*cursor == ';') {
                    cursor += 2;
                    state = PARSING_SET_START;
                    break;
                }
        } break;
        default:
            assert(false);
        }
    }

    return max_red * max_green * max_blue;
}
int main(void)
{
    size_t sum = 0;
    size_t sum2 = 0;

    char line[1024];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t line_len = strnlen(line, sizeof(line));
        assert(line_len > 0);
        line[--line_len] = '\0';

        size_t game_id;
        if (is_game_possible(line, line_len, &game_id)) {
            sum += game_id;
        }

        sum2 += compute_max(line, line_len, &game_id);
    }

    printf("sum: %zu\n", sum);
    printf("sum2: %zu\n", sum2);
}

