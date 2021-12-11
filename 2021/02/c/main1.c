#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct cmd {
    char direction[32];
    int value;
};

struct position {
    int horizontal;
    int depth;
};

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define CMD_FORWARD "forward"
#define CMD_FORWARD_SIZE ARRAY_SIZE(CMD_FORWARD)

#define CMD_UP "up"
#define CMD_UP_SIZE ARRAY_SIZE(CMD_UP)

#define CMD_DOWN "down"
#define CMD_DOWN_SIZE ARRAY_SIZE(CMD_DOWN)

#define MIN(x, y) ((x) < (y) ? (x) : (y))

int main()
{
    struct cmd cmd = {0};
    struct position pos = {0};
    while(scanf("%s %d", cmd.direction, &cmd.value) == 2) {
        if(strncmp(cmd.direction, CMD_FORWARD, MIN(CMD_FORWARD_SIZE, ARRAY_SIZE(cmd.direction))) == 0) {
            pos.horizontal += cmd.value;
        } else if (strncmp(cmd.direction, CMD_UP, MIN(CMD_UP_SIZE, ARRAY_SIZE(cmd.direction))) == 0) {
            pos.depth -= cmd.value;
        } else if (strncmp(cmd.direction, CMD_DOWN, MIN(CMD_DOWN_SIZE, ARRAY_SIZE(cmd.direction))) == 0) {
            pos.depth += cmd.value;
        } else {
            fprintf(stderr, "error: invalid command direction '%s'\n", cmd.direction);
            exit(EXIT_FAILURE);
        }
    }
    printf("%d\n", pos.horizontal * pos.depth);
}

