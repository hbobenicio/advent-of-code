#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <errno.h>

#define ARRAY_SIZE(XS) (sizeof(XS) / sizeof((XS)[0]))

struct numbers {
    size_t* items;
    size_t len;
    size_t cap;
};

struct card {
    size_t id;
    size_t count;
    struct numbers winning;
    struct numbers nums;
};

struct cards {
    struct card* items;
    size_t len;
    size_t cap;
};

struct input {
    struct cards cards;
};

struct card card_parse(const char* line);
size_t card_score(struct card* card);
size_t card_matches(struct card* card);

void cards_free(struct cards* cards);
void numbers_free(struct numbers* numbers);
void cards_append(struct cards* cards, struct card card);
void numbers_append(struct numbers* numbers, size_t x);

int main(void)
{
    struct input input = {0};

    char line[512];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t line_len = strnlen(line, sizeof(line));
        assert(line_len > 0);
        line[--line_len] = '\0';
        if (line_len == 0) {
            continue;
        }

        const struct card card = card_parse(line);
        cards_append(&input.cards, card);
    }

    size_t total_cards_count = input.cards.len;
    for (size_t i = 0; i < input.cards.len; i++) {
        struct card* card = &input.cards.items[i];
        //size_t score = card_score(card);
        size_t matches = card_matches(card);
        size_t additional_cards = matches * card->count;
        total_cards_count += additional_cards;

        printf("Card %zu: Count: %zu, Matches: %zu, Additional Cards: %zu\n",
               card->id, card->count, matches, additional_cards);

        for (size_t q = 0; q < card->count; q++) {
            for (size_t j = i+1, qtd = matches; j < input.cards.len && qtd > 0; j++, qtd--) {
                struct card* next_card = &input.cards.items[j];
                next_card->count++;
            }
        }
    }
    printf("----\nTotal Cards Count: %zu\n", total_cards_count);

    for (size_t i = 0; i < input.cards.len; i++) {
        numbers_free(&input.cards.items[i].winning);
        numbers_free(&input.cards.items[i].nums);
    }
    cards_free(&input.cards);
}

struct card card_parse(const char* line)
{
    struct card card = {
        .count = 1,
    };

    int rc = sscanf(line, "Card %zu", &card.id);
    assert(rc == 1);

    const char* colon = strchr(line, ':');
    assert(colon != NULL);

    const char* pipe = strchr(line, '|');
    assert(pipe != NULL);

    char* winning_buf = strndup(colon + 2, pipe - colon - 3);
    assert(winning_buf != NULL);

    char* nums_buf = strdup(pipe + 2);
    assert(nums_buf != NULL);

    char* cursor = winning_buf;
    while (true) {
        char* end = NULL;
        size_t x = strtoul(cursor, &end, 10);
        assert(errno == 0);
        if (end == cursor) {
            break;
        }
        numbers_append(&card.winning, x);
        cursor = end;
    }

    cursor = nums_buf;
    while (true) {
        char* end = NULL;
        size_t x = strtoul(cursor, &end, 10);
        assert(errno == 0);
        if (end == cursor) {
            break;
        }
        numbers_append(&card.nums, x);
        cursor = end;
    }

    free(nums_buf);
    free(winning_buf);
    return card;
}

size_t card_score(struct card* card)
{
    assert(card != NULL);

    size_t matches_count = card_matches(card);
    if (matches_count == 0) {
        return 0;
    }
    return (size_t) pow(2, matches_count - 1);
}

size_t card_matches(struct card* card)
{
    assert(card != NULL);

    size_t matches_count = 0;
    for (size_t i = 0; i < card->winning.len; i++) {
        size_t w = card->winning.items[i];
        for (size_t j = 0; j < card->nums.len; j++) {
            size_t x = card->nums.items[j];
            if (w == x) {
                matches_count++;
                break; // go to the next winning number
            }
        }
    }
    return matches_count;
}

void cards_free(struct cards* cards)
{
    assert(cards != NULL);
    free(cards->items);
    cards->items = NULL;
    cards->cap = cards->len = 0;
}

void numbers_free(struct numbers* numbers)
{
    assert(numbers != NULL);
    free(numbers->items);
    numbers->items = NULL;
    numbers->cap = numbers->len = 0;
}

void cards_append(struct cards* cards, struct card card)
{
    assert(cards != NULL);
    assert(cards->len <= cards->cap);

    if (cards->len >= cards->cap) {
        const size_t new_cap = (cards->cap == 0) ? 8 : (cards->cap * 2);
        cards->items = realloc(cards->items, new_cap * sizeof(cards->items[0]));
        assert(cards->items != NULL);
        cards->cap = new_cap;
    }
    cards->items[cards->len] = card;
    cards->len++;
}

void numbers_append(struct numbers* numbers, size_t x)
{
    assert(numbers != NULL);
    assert(numbers->len <= numbers->cap);

    if (numbers->len >= numbers->cap) {
        const size_t new_cap = (numbers->cap == 0) ? 8 : (numbers->cap * 2);
        numbers->items = realloc(numbers->items, new_cap * sizeof(numbers->items[0]));
        assert(numbers->items != NULL);
        numbers->cap = new_cap;
    }
    numbers->items[numbers->len] = x;
    numbers->len++;
}

