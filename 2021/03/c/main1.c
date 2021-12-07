#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sys/types.h>

int main()
{
    char line[16] = {0};
    size_t line_len = 0;
    size_t line_count = 0;
    size_t* histogram = NULL;
    while (scanf("%s", line) == 1) {
        if (line_len == 0) {
            line_len = strnlen(line, sizeof(line) / sizeof(line[0]));
            histogram = calloc(line_len, sizeof(size_t));
        }
        line_count++;
        for (size_t i = 0; i < line_len; i++) {
            if (line[i] == '1') {
                histogram[i]++;
            }
        }
    }
    unsigned long long gamma = 0, epsilon = 0;
    for (ssize_t i = line_len - 1, power = 0; i >= 0; i--, power++) {
        if (histogram[i] > (line_count / 2)) {
            // gamma 1, epsilon 0
            gamma += pow(2.0, power);
        } else {
            // gamma 0, epsilon 1
            epsilon += pow(2.0, power);
        }
    }
    printf("%llu\n", gamma * epsilon);
    free(histogram);
}
