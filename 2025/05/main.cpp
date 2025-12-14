#include <iostream>
#include <print>
#include <vector>
#include <tuple>
#include <array>
#include <optional>
#include <cassert>
#include <inttypes.h>

using Ingredient_ID = std::uint64_t;
using Ingredient_ID_Range = std::array<Ingredient_ID, 2>;

struct Input {
    std::vector<Ingredient_ID_Range> fresh_ingredients;
    std::vector<Ingredient_ID> available_ingredients;

    void read_and_parse();
};

namespace A {
    uint64_t solve(const Input& input);
}

namespace B {
    // TODO
}

int main(void)
{
    Input input;
    input.read_and_parse();

    std::println("A: {}", A::solve(input));
    // std::println("B: {}", B::solve(input));
}

void Input::read_and_parse()
{
    enum class ReadingMode {
        Ranges,
        Ids,
    } reading_mode = ReadingMode::Ranges;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (reading_mode == ReadingMode::Ranges && line.size() == 0) {
            reading_mode = ReadingMode::Ids;
            continue;
        }
        switch (reading_mode) {
            case ReadingMode::Ranges: {
                Ingredient_ID_Range range;
                int rc = std::sscanf(line.c_str(), "%" PRIu64 "-%" PRIu64, &range[0], &range[1]);
                assert(rc == 2);

                this->fresh_ingredients.emplace_back(range);
            } break;
            
            case ReadingMode::Ids: {
                char* endptr = nullptr;
                Ingredient_ID ingredient = static_cast<Ingredient_ID>(std::strtoull(line.c_str(), &endptr, 10));
                assert(*endptr == '\0');

                this->available_ingredients.emplace_back(ingredient);
            } break;
        
            default:
                assert(false);
                break;
        }
    }
}

uint64_t A::solve(const Input& input)
{
    uint64_t fresh_count = 0;

    for (Ingredient_ID ingredient: input.available_ingredients) {
        for (const Ingredient_ID_Range& fresh_range: input.fresh_ingredients) {

            // an ingredient is fresh if it's inside any fresh range
            if (fresh_range.at(0) <= ingredient && ingredient <= fresh_range.at(1)) {
                fresh_count++;
                break;
            }
        }
    }
    return fresh_count;
}
