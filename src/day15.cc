#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/split.hpp>

#include <fmt/core.h>

#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/drop_last.hpp>
#include <range/v3/view/enumerate.hpp>

#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using Value = int64_t;
using Ingredient = std::array<Value, 5>;
using Ingredients = std::vector<Ingredient>;

void process(Ingredients const& ingredients)
{
    std::vector<Value> distribution(ingredients.size(), 0);

    auto apply = [&ingredients, &distribution]() -> Ingredient {
        Ingredient total = {0};
        for (auto const& [idx_i, i] : ranges::views::enumerate(ingredients)) {
            for (auto const& [idx_t, val] : total | ranges::views::enumerate) {
                val += distribution.at(idx_i) * i.at(idx_t);
            }
        }
        for (auto& val : total) {
            if (val < 0) {
                val = 0;
            }
        }
        return total;
    };

    Value max_score1 = 0;
    Value max_score2 = 0;

    for (;;) {
        distribution.front() = 0;
        Value sum = ranges::accumulate(distribution, 0);
        Value max = 100;
        if (sum == max) {
            max = *ranges::max_element(distribution);
        }
        for (unsigned idx = 1; idx < distribution.size(); ++idx) {
            Value& val = distribution.at(idx);
            ++val;
            if (val > max) {
                val = 0;
            } else {
                break;
            }
        }

        Value rest = 100 - ranges::accumulate(distribution, 0);
        distribution.front() = rest;

        auto total = apply();
        max_score1 = std::max(
            max_score1, ranges::accumulate(total | ranges::views::drop_last(1), (Value)1, std::multiplies<Value>()));
        if (total.back() == 500) {
            max_score2 = std::max(
                max_score2,
                ranges::accumulate(total | ranges::views::drop_last(1), (Value)1, std::multiplies<Value>()));
        }

        if (distribution.front() == 100) {
            break;
        }
    }

    fmt::print("1: {}\n", max_score1);
    fmt::print("2: {}\n", max_score2);
}

int main()
{
    Ingredients ingredients;

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) break;

        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of(" ,:"), boost::token_compress_on);

        Ingredient i = {0};
        i.at(0) = std::stol(parts.at(2));
        i.at(1) = std::stol(parts.at(4));
        i.at(2) = std::stol(parts.at(6));
        i.at(3) = std::stol(parts.at(8));
        i.at(4) = std::stol(parts.at(10));
        ingredients.push_back(std::move(i));
    }

    process(ingredients);
}
