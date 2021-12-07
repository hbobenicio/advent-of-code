#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct {
    size_t nums[5][5];
} Board;

typedef struct BoardList {
    Board board;
    struct BoardList* next;
} BoardList;

void board_list_free(BoardList* list)
{
    while (list) {
        BoardList* next = list->next;
        free(list);
        list = next;
    }
}

int main()
{
    char line[1024] = {0};
    size_t nums[1024] = {0}, nums_count = 0;
    assert(scanf("%zu", &nums[nums_count++]) == 1);
    while (scanf(",%zu", &nums[nums_count]) == 1) {
        nums_count++;
    }

    assert(fgets(line, sizeof(line), stdin) != NULL);
    assert(strlen(line) == 1);

    assert(fgets(line, sizeof(line), stdin) != NULL);
    assert(strlen(line) == 1);

    BoardList* board_list = NULL;
    while (!feof(stdin)) {
        BoardList* new_board_node = calloc(1, sizeof(BoardList));
        for (size_t i = 0; i < 5; i++) {
            for (size_t j = 0; j < 5; j++) {
                assert(scanf("%zu", &new_board_node->board.nums[i][j]) == 1);
            }
        }
        if (board_list == NULL) {
            board_list = new_board_node;
        } else {
            new_board_node->next = board_list;
            board_list = new_board_node;
        }
        getchar();
        getchar();
    }

    // start game simulation
    for (size_t n = 0; n < nums_count; n++) {
        size_t num = nums[n];

        // check if any board won
    }

    board_list_free(board_list);
}

