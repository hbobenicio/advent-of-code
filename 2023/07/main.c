#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define CARDS_TOTAL 13

size_t card_hash(char card);
size_t card_hash_with_joker(char card);
char card_unhash_with_joker(size_t hash);
#define PART_B_JOKER_HASH 0

#define HAND_CARDS_LEN 5
#define HAND_CARDS_CAP (HAND_CARDS_LEN + 1)

typedef struct {
    char cards[HAND_CARDS_CAP];
    size_t bid;
    bool has_joker;
} Hand;

Hand hand_parse(const char* line);
bool hand_has_joker(const Hand* hand);

typedef enum {
    HAND_TYPE_HIGH_CARD,
    HAND_TYPE_ONE_PAIR,
    HAND_TYPE_TWO_PAIR,
    HAND_TYPE_THREE_OF_A_KIND,
    HAND_TYPE_FULL_HOUSE,
    HAND_TYPE_FOUR_OF_A_KIND,
    HAND_TYPE_FIVE_OF_A_KIND,
} HandType;

HandType hand_type(const Hand* hand);
HandType hand_type_with_joker(const Hand* hand);
const char* hand_type_to_cstr(HandType hand_type);

typedef struct {
    Hand* items;
    size_t len;
    size_t cap;
} Hands;

void hands_append(Hands* hands, Hand hand);
void hands_free(Hands* hands);
int hand_cmp_rank_desc(const void* a, const void* b);
int hand_cmp_rank_desc_with_joker(const void* a, const void* b);

typedef struct {
    Hands hands;
} Input;

 void input_free(Input* input);
Input input_parse(void);

int main(void)
{
    Input input = input_parse();

    printf("==== Part A ====\n");
    {
        qsort(input.hands.items, input.hands.len, sizeof(input.hands.items[0]), hand_cmp_rank_desc);
        size_t total_winnings = 0;
        for (size_t i = 0; i < input.hands.len; i++) {
            const Hand* hand = &input.hands.items[i];
            size_t hand_rank = input.hands.len - i;
            size_t hand_winnings = hand_rank * hand->bid;
            total_winnings += hand_winnings;
            //printf("Hand %zu: Cards: %s Bid: %zu Type: %s Rank: %zu Winnings: %zu\n",
            //       i+1, hand->cards, hand->bid, hand_type_to_cstr(hand_type(hand)), hand_rank, hand_winnings);
        }
        printf("Total Winnings: %zu\n", total_winnings);
    }

    printf("\n==== Part B ====\n");
    {
        qsort(input.hands.items, input.hands.len, sizeof(input.hands.items[0]), hand_cmp_rank_desc_with_joker);
        size_t total_winnings = 0;
        for (size_t i = 0; i < input.hands.len; i++) {
            const Hand* hand = &input.hands.items[i];
            const size_t hand_rank = input.hands.len - i;
            const size_t hand_winnings = hand_rank * hand->bid;

            total_winnings += hand_winnings;

            printf("Hand %04zu: Cards: %s HasJoker: %s Bid: %03zu Rank: %04zu Winnings: %zu Type: %s\n",
                   i+1,
                   hand->cards,
                   hand->has_joker ? "true " : "false",
                   hand->bid,
                   hand_rank,
                   hand_winnings,
                   hand_type_to_cstr(hand_type_with_joker(hand))
            );
        }
        printf("----\n");
        printf("Total Winnings: %zu\n", total_winnings);
    }
    input_free(&input);
}

Input input_parse(void)
{
    Input input = {0};

    char line[512];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t line_len = strnlen(line, sizeof(line));
        assert(line_len > 0);
        line[--line_len] = '\0';
        if (line_len == 0) {
            continue;
        }
        hands_append(&input.hands, hand_parse(line));
    }

    return input;
}

Hand hand_parse(const char* line)
{
    Hand hand = {0};

    int rc = sscanf(line, "%s %zu", hand.cards, &hand.bid);
    assert(rc == 2);

    hand.has_joker = hand_has_joker(&hand);

    return hand;
}

bool hand_has_joker(const Hand* hand)
{
    for (size_t i = 0; i < HAND_CARDS_LEN; i++) {
        if (hand->cards[i] == 'J') {
            return true;
        }
    }
    return false;
}

int hand_cmp_rank_desc(const void* a, const void* b)
{
    const Hand* h1 = a;
    const Hand* h2 = b;

    HandType h1_type = hand_type(h1);
    HandType h2_type = hand_type(h2);
    
    int diff = h2_type - h1_type;
    if (diff != 0) {
        return diff;
    }

    for (size_t i = 0; i < HAND_CARDS_LEN; i++) {
        char c1 = h1->cards[i];
        char c2 = h2->cards[i];
        diff = card_hash(c2) - card_hash(c1);
        if (diff != 0) {
            return diff;
        }
    }

    return 0;
}

int hand_cmp_rank_desc_with_joker(const void* a, const void* b)
{
    const Hand* h1 = a;
    const Hand* h2 = b;

    HandType h1_type = hand_type_with_joker(h1);
    HandType h2_type = hand_type_with_joker(h2);
    
    int diff = h2_type - h1_type;
    if (diff != 0) {
        return diff;
    }

    for (size_t i = 0; i < HAND_CARDS_LEN; i++) {
        char c1 = h1->cards[i];
        char c2 = h2->cards[i];
        diff = card_hash_with_joker(c2) - card_hash_with_joker(c1);
        if (diff != 0) {
            return diff;
        }
    }

    return 0;
}

HandType hand_type(const Hand* hand)
{
    size_t histogram[CARDS_TOTAL] = {0};
    for (size_t i = 0; i < HAND_CARDS_LEN; i++) {
        const char card = hand->cards[i];
        size_t hash = card_hash(card);
        assert(hash < CARDS_TOTAL);
        histogram[hash]++;
    }

    bool found_two = false;
    bool found_three = false;
    size_t pair_count = 0;
    for (size_t i = 0; i < CARDS_TOTAL; i++) {
        const size_t count = histogram[i];
        if (count == 0) continue;
        if (count == 5) {
            return HAND_TYPE_FIVE_OF_A_KIND;
        }
        if (count == 4) {
            return HAND_TYPE_FOUR_OF_A_KIND;
        }
        if (count == 3) {
            found_three = true;
            continue;
        }
        if (count == 2) {
            found_two = true;
            pair_count++;
            continue;
        }
    }
    if (found_three && found_two) {
        return HAND_TYPE_FULL_HOUSE;
    }
    if (found_three) {
        return HAND_TYPE_THREE_OF_A_KIND;
    }
    if (found_two) {
        if (pair_count == 2) return HAND_TYPE_TWO_PAIR;
        assert(pair_count == 1);
        return HAND_TYPE_ONE_PAIR;
    }
    return HAND_TYPE_HIGH_CARD;
}

/**
 * the reasoning about this is easily infered by writing down the truth-table of hand-types and possible improvements.
 */
HandType hand_type_with_joker(const Hand* hand)
{
    if (!hand->has_joker) {
        return hand_type(hand);
    }

    size_t histogram[CARDS_TOTAL] = {0};
    for (size_t i = 0; i < HAND_CARDS_LEN; i++) {
        const char card = hand->cards[i];
        size_t hash = card_hash_with_joker(card);
        assert(hash < CARDS_TOTAL);
        histogram[hash]++;
    }

    HandType std_type = hand_type(hand);
    switch (std_type) {
        case HAND_TYPE_FIVE_OF_A_KIND:
        case HAND_TYPE_FOUR_OF_A_KIND:
        case HAND_TYPE_FULL_HOUSE:
            return HAND_TYPE_FIVE_OF_A_KIND;

        case HAND_TYPE_THREE_OF_A_KIND:
            return HAND_TYPE_FOUR_OF_A_KIND;

        case HAND_TYPE_TWO_PAIR: {
            if (histogram[PART_B_JOKER_HASH] == 2) {
                return HAND_TYPE_FOUR_OF_A_KIND;
            }
            if (histogram[PART_B_JOKER_HASH] == 1) {
                return HAND_TYPE_FULL_HOUSE;
            }
            assert(false);
        } break;

        case HAND_TYPE_ONE_PAIR:
            return HAND_TYPE_THREE_OF_A_KIND;

        case HAND_TYPE_HIGH_CARD:
            return HAND_TYPE_ONE_PAIR;
    }
    assert(false);
}

const char* hand_type_to_cstr(HandType hand_type)
{
    switch (hand_type) {
        case HAND_TYPE_HIGH_CARD: return "High Card";
        case HAND_TYPE_ONE_PAIR: return "One Pair";
        case HAND_TYPE_TWO_PAIR: return "Two Pair";
        case HAND_TYPE_THREE_OF_A_KIND: return "Three of a Kind";
        case HAND_TYPE_FULL_HOUSE: return "Full House";
        case HAND_TYPE_FOUR_OF_A_KIND: return "Four of a Kind";
        case HAND_TYPE_FIVE_OF_A_KIND: return "Five of a Kind";
    }
    assert(false);
}

size_t card_hash(char card)
{
    switch (card) {
    case '2': return 0;
    case '3': return 1;
    case '4': return 2;
    case '5': return 3;
    case '6': return 4;
    case '7': return 5;
    case '8': return 6;
    case '9': return 7;
    case 'T': return 8;
    case 'J': return 9;
    case 'Q': return 10;
    case 'K': return 11;
    case 'A': return 12;
    }
    assert(false);
}

size_t card_hash_with_joker(char card)
{
    switch (card) {
    case 'J': return 0;
    case '2': return 1;
    case '3': return 2;
    case '4': return 3;
    case '5': return 4;
    case '6': return 5;
    case '7': return 6;
    case '8': return 7;
    case '9': return 8;
    case 'T': return 9;
    case 'Q': return 10;
    case 'K': return 11;
    case 'A': return 12;
    }
    assert(false);
}

char card_unhash(size_t hash)
{
    switch (hash) {
    case 0:  return '2';
    case 1:  return '3';
    case 2:  return '4';
    case 3:  return '5';
    case 4:  return '6';
    case 5:  return '7';
    case 6:  return '8';
    case 7:  return '9';
    case 8:  return 'T';
    case 9:  return 'J';
    case 10: return 'Q';
    case 11: return 'K';
    case 12: return 'A';
    }
    assert(false);
}

char card_unhash_with_joker(size_t hash)
{
    switch (hash) {
    case 0:  return 'J';
    case 1:  return '2';
    case 2:  return '3';
    case 3:  return '4';
    case 4:  return '5';
    case 5:  return '6';
    case 6:  return '7';
    case 7:  return '8';
    case 8:  return '9';
    case 9:  return 'T';
    case 10: return 'Q';
    case 11: return 'K';
    case 12: return 'A';
    }
    assert(false);
}

void hands_append(Hands* hands, Hand hand)
{
    if (hands->len >= hands->cap) {
        hands->cap = (hands->cap == 0) ? 8 : (hands->cap * 2);
        hands->items = realloc(hands->items, hands->cap * sizeof(hands->items[0]));
    }
    hands->items[hands->len++] = hand;
}

void hands_free(Hands* hands)
{
    free(hands->items);
    hands->items = NULL;
    hands->cap = hands->len = 0;
}

void input_free(Input* input)
{
    hands_free(&input->hands);
}

