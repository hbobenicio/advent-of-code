#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct {
    char* values;
    size_t count;
    size_t capacity;
} Instructions;

void instructions_free(Instructions* is)
{
    free(is->values);
    is->values = NULL;
    is->count = is->count = 0;
}

typedef struct {
    char from[5];
    char left[5];
    char right[5];
} MapValue;

typedef struct {
    MapValue* values;
    size_t count;
    size_t capacity;
} Map;

void map_append(Map* map, MapValue value)
{
    if (map->count >= map->capacity) {
        map->capacity = (map->capacity == 0) ? 8 : map->capacity * 2;
        map->values = realloc(map->values, map->capacity * sizeof(map->values[0]));
    }
    map->values[map->count] = value;
    map->count++;
}

void map_free(Map* map)
{
    free(map->values);
    map->values = NULL;
    map->count = map->capacity = 0;
}

typedef struct {
    Instructions instructions;
    Map map;
} Input;

void input_free(Input* input)
{
    map_free(&input->map);
    instructions_free(&input->instructions);
}

Input input_parse(void)
{
    Input input = {0};

    char line[4096] = {0};

    // Instructions Parsing
    input.instructions.capacity = 4096;
    input.instructions.values = calloc(input.instructions.capacity, sizeof(input.instructions.values[0]));
    assert(input.instructions.values != NULL);
    {
        const char* rc = fgets(input.instructions.values, input.instructions.capacity, stdin);
        assert(rc != NULL);
        input.instructions.count = strnlen(input.instructions.values, input.instructions.capacity);
        assert(input.instructions.count > 0);
        input.instructions.values[input.instructions.count - 1] = '\0';
        input.instructions.count--;
    }
    // skip the new line
    {
        const char* rc = fgets(line, sizeof(line), stdin);
        assert(rc != NULL);
    }

    // Mapping Values Parsing
    MapValue map_value = {0};
    while (scanf("%s = (%s %s)", map_value.from, map_value.left, map_value.right) == 3) {
        map_value.left[strlen(map_value.left) - 1] = '\0';
        map_value.right[strlen(map_value.right) - 1] = '\0';
        assert(strlen(map_value.from) == 3);
        assert(strlen(map_value.left) == 3);
        assert(strlen(map_value.right) == 3);
        map_append(&input.map, map_value);
        memset(&map_value, 0, sizeof(map_value));
    }

    return input;
}

size_t part_a(const Input* input)
{
    size_t steps_count = 0;

    size_t current_instruction_idx = 0;
    char current_state[5] = "AAA";
    while (strcmp(current_state, "ZZZ") != 0) {
        steps_count++;
        char instruction = input->instructions.values[current_instruction_idx];
        for (size_t i = 0; i < input->map.count; i++) {
            const MapValue* mv = &input->map.values[i];
            if (strcmp(mv->from, current_state) == 0) {
                strcpy(current_state, instruction == 'L' ? mv->left : mv->right);
                current_instruction_idx = (current_instruction_idx + 1) % input->instructions.count;
                break;
            }
        }
    }

    return steps_count;
}

int main(void)
{
    Input input = input_parse();
    
    printf("==== Part A ====\n");
    printf("Instructions: [%zu] %s\n", input.instructions.count, input.instructions.values);
    for (size_t i = 0; i < input.map.count; i++) {
        const MapValue* map_value = &input.map.values[i];
        printf("%s = (%s, %s)\n", map_value->from, map_value->left, map_value->right);
    }
    printf("----\n");
    printf("Total Steps: %zu\n", part_a(&input));

    input_free(&input);
}

