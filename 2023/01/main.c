#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define ARRAY_SIZE(XS) (sizeof(XS) / sizeof((XS)[0]))

struct number {
    const char* cstr;
    size_t len;
    size_t value;
};

static const struct number numbers[] = {
    { .cstr = "one",   .len = sizeof("one")   - 1, .value = 1 },
    { .cstr = "two",   .len = sizeof("two")   - 1, .value = 2 },
    { .cstr = "three", .len = sizeof("three") - 1, .value = 3 },
    { .cstr = "four",  .len = sizeof("four")  - 1, .value = 4 },
    { .cstr = "five",  .len = sizeof("five")  - 1, .value = 5 },
    { .cstr = "six",   .len = sizeof("six")   - 1, .value = 6 },
    { .cstr = "seven", .len = sizeof("seven") - 1, .value = 7 },
    { .cstr = "eight", .len = sizeof("eight") - 1, .value = 8 },
    { .cstr = "nine",  .len = sizeof("nine")  - 1, .value = 9 },
};

size_t calibration_value(const char* line, size_t line_len);
size_t ascii_char_to_num(char c);

int main(void)
{
    size_t total_calibration_value = 0;

    char line[512];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t line_len = strnlen(line, sizeof(line));
        assert(line_len > 0);
        line[--line_len] = '\0';
        
        total_calibration_value += calibration_value(line, line_len);
    }

    printf("sum: %zu\n", total_calibration_value);
}

size_t calibration_value(const char* line, size_t line_len)
{
    size_t result = 0;

    for (const char* start = line; *start != '\0'; start++) {
        if (isdigit(*start)) {
            result += 10 * ascii_char_to_num(*start);
            break;
        }
        for (size_t i = 0; i < ARRAY_SIZE(numbers); i++) {
            if (strncmp(start, numbers[i].cstr, numbers[i].len) == 0) {
                result += 10 * numbers[i].value;
                goto scan_start_done;
            }
        }
    }
scan_start_done:

    for (const char* end = line + line_len - 1; end >= line; end--) {
        if (isdigit(*end)) {
            result += ascii_char_to_num(*end);
            break;
        }
        for (size_t i = 0; i < ARRAY_SIZE(numbers); i++) {
            const char* start = end - numbers[i].len + 1;
            if (start < line) {
                continue;
            }
            if (strncmp(start, numbers[i].cstr, numbers[i].len) == 0) {
                result += numbers[i].value;
                goto scan_end_done;
            }
        }
    }
scan_end_done:

    return result;
}

size_t ascii_char_to_num(char c)
{
    assert(c >= '0' && c <= '9');
    return (size_t) c - '0';
}

