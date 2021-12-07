#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

const char* parse_sizes(const char* s, size_t** xs, size_t* len);

typedef struct {
    size_t* values;
    size_t len;
} Seeds;

 void seeds_free(Seeds* seeds);
Seeds seeds_parse(const char* line);

typedef struct {
    size_t dest_start;
    size_t source_start;
    size_t len;
} MapEntry;

MapEntry map_entry_parse(const char* s);

typedef struct {
    MapEntry* values;
    size_t len;
    size_t cap;
} Map;

  void map_free(Map* map);
  void map_append(Map* map, MapEntry value);
size_t map_get(const Map* map, size_t key);

typedef struct {
    Seeds seeds;
    Map seed_to_soil;
    Map soil_to_fertilizer;
    Map fertilizer_to_water;
    Map water_to_light;
    Map light_to_temperature;
    Map temperature_to_humidity;
    Map humidity_to_location;
} Input;

Input input_parse(FILE* stream);
 void input_free(Input* input);

size_t a_solve(Input* input);
size_t b_solve(Input* input);

int main(void)
{
    Input input = input_parse(stdin);

    const size_t a = a_solve(&input);
    printf("Part A: %zu\n", a);

    const size_t b = b_solve(&input);
    printf("Part B: %zu\n", b);

    input_free(&input);
}

size_t a_solve(Input* input)
{
    assert(input != NULL);

    size_t location_min = ULONG_MAX;
    for (size_t s = 0; s < input->seeds.len; s++) {
        const size_t seed = input->seeds.values[s];
        const size_t soil = map_get(&input->seed_to_soil, seed);
        const size_t fertilizer = map_get(&input->soil_to_fertilizer, soil);
        const size_t water = map_get(&input->fertilizer_to_water, fertilizer);
        const size_t light = map_get(&input->water_to_light, water);
        const size_t temperature = map_get(&input->light_to_temperature, light);
        const size_t humidity = map_get(&input->temperature_to_humidity, temperature);
        const size_t location = map_get(&input->humidity_to_location, humidity);

        /*
        printf("Seed: %zu, soil: %zu, fertilizer: %zu, water: %zu, light: %zu, temperature: %zu, humidity: %zu, location: %zu\n",
               seed, soil, fertilizer, water, light, temperature, humidity, location);
        */

        location_min = MIN(location_min, location);
    }
    return location_min;
}

size_t b_solve(Input* input)
{
    assert(input != NULL);

    size_t location_min = ULONG_MAX;
    for (size_t s = 0; s < input->seeds.len; s += 2) {
        const size_t seed_start = input->seeds.values[s];
        const size_t seed_len = input->seeds.values[s+1];
        for (size_t seed = seed_start; seed < seed_start + seed_len; seed++) {
            
            const size_t soil = map_get(&input->seed_to_soil, seed);
            const size_t fertilizer = map_get(&input->soil_to_fertilizer, soil);
            const size_t water = map_get(&input->fertilizer_to_water, fertilizer);
            const size_t light = map_get(&input->water_to_light, water);
            const size_t temperature = map_get(&input->light_to_temperature, light);
            const size_t humidity = map_get(&input->temperature_to_humidity, temperature);
            const size_t location = map_get(&input->humidity_to_location, humidity);

            /*
            printf("Seed: %zu, soil: %zu, fertilizer: %zu, water: %zu, light: %zu, temperature: %zu, humidity: %zu, location: %zu\n",
                   seed, soil, fertilizer, water, light, temperature, humidity, location);
            */

            location_min = MIN(location_min, location);
        }
    }
    return location_min;
}


Input input_parse(FILE* stream)
{
    Input input = {0};

    // for each line
    char line[512];
    size_t line_count = 0;
    Map* map = NULL;
    while (fgets(line, sizeof(line), stream) != NULL) {
        size_t line_len = strnlen(line, sizeof(line));
        assert(line_len > 0);
        line[--line_len] = '\0';
        line_count++;

        if (line_len == 0) {
            continue;
        }

        if (line_count == 1) {
            input.seeds = seeds_parse(line);
            continue;
        }

        if (strcmp(line, "seed-to-soil map:") == 0) {
            map = &input.seed_to_soil;
        } else if (strcmp(line, "soil-to-fertilizer map:") == 0) {
            map = &input.soil_to_fertilizer;
        } else if (strcmp(line, "fertilizer-to-water map:") == 0) {
            map = &input.fertilizer_to_water;
        } else if (strcmp(line, "water-to-light map:") == 0) {
            map = &input.water_to_light;
        } else if (strcmp(line, "light-to-temperature map:") == 0) {
            map = &input.light_to_temperature;
        } else if (strcmp(line, "temperature-to-humidity map:") == 0) {
            map = &input.temperature_to_humidity;
        } else if (strcmp(line, "humidity-to-location map:") == 0) {
            map = &input.humidity_to_location;
        } else {
            // map entry line
            map_append(map, map_entry_parse(line));
        }
    }

    return input;
}

void input_free(Input* input)
{
    assert(input != NULL);
    seeds_free(&input->seeds);
    map_free(&input->seed_to_soil);
    map_free(&input->soil_to_fertilizer);
    map_free(&input->fertilizer_to_water);
    map_free(&input->water_to_light);
    map_free(&input->light_to_temperature);
    map_free(&input->temperature_to_humidity);
    map_free(&input->humidity_to_location);
}

void seeds_free(Seeds* seeds)
{
    assert(seeds != NULL);
    free(seeds->values);
    seeds->values = NULL;
    seeds->len = 0;
}

Seeds seeds_parse(const char* line)
{
    assert(line != NULL);

    Seeds seeds;
    parse_sizes(strchr(line, ':') + 2, &seeds.values, &seeds.len);

    return seeds;
}

void map_free(Map* map)
{
    assert(map != NULL);
    free(map->values);
    map->values = NULL;
    map->len = map->cap = 0;
}

void map_append(Map* map, MapEntry value)
{
    assert(map != NULL);
    if (map->len >= map->cap) {
        map->cap = (map->cap == 0) ? 8 : (map->cap * 8);
        map->values = realloc(map->values, map->cap * sizeof(map->values[0]));
        assert(map->values != NULL);
    }
    map->values[map->len] = value;
    map->len++;
}

size_t map_get(const Map* map, size_t key)
{
    assert(map != NULL);

    for (size_t i = 0; i < map->len; i++) {
        const MapEntry* e = &map->values[i];
        if (e->source_start <= key && key <= e->source_start + e->len) {
            const size_t diff = key - e->source_start;
            return e->dest_start + diff;
        }
    }

    return key;
}

MapEntry map_entry_parse(const char* s)
{
    MapEntry entry;
    int rc = sscanf(s, "%zu %zu %zu", &entry.dest_start, &entry.source_start, &entry.len);
    assert(rc == 3);
    return entry;
}

void seeds_append(Seeds* seeds, size_t x)
{
    assert(seeds->len < 128);
    seeds->values[seeds->len++] = x;
}

const char* parse_sizes(const char* s, size_t** xs, size_t* len)
{
    assert(s != NULL);
    assert(xs != NULL);
    assert(len != NULL);

    *xs = NULL;
    *len = 0;
    size_t cap = 0;

    const char* cursor = s;
    while (true) {
        char* end = NULL;
        size_t x = strtoull(cursor, &end, 10);
        assert(errno == 0);

        // end of input
        if (end == cursor) {
            break;
        }

        // append
        if (*len >= cap) {
            cap = (cap == 0) ? 8 : (cap * 2);
            *xs = realloc(*xs, cap * sizeof(**xs));
            assert(*xs != NULL);
        }
        (*xs)[*len] = x;
        *len += 1;

        cursor = end;
    }

    return cursor;
}

