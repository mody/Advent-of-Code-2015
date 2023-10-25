#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/split.hpp>

#include <fmt/core.h>

#include <functional>
#include <range/v3/view/all.hpp>

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <range/v3/view/drop_last.hpp>
#include <range/v3/view/enumerate.hpp>
#include <string>
#include <vector>

using Value = int64_t;
using Ingredient = std::array<Value, 5>;
using Ingredients = std::vector<Ingredient>;

void part1(Ingredients const& ingredients)
{
    std::vector<Value> distribution(ingredients.size(), 0);

    auto apply = [&ingredients, &distribution]() -> Value {
        Ingredient total = {0};
        for (auto const& [idx_i, i] : ranges::views::enumerate(ingredients)) {
            for (auto const& [idx_t, val] : total | ranges::views::drop_last(1) | ranges::views::enumerate) {
                val += distribution.at(idx_i) * i.at(idx_t);
            }
        }
        for (auto& val : total) {
            if (val < 0) {
                val = 0;
            }
        }
        total.back() = 1; // ignoring calories
        return std::accumulate(total.begin(), total.end(), 1, std::multiplies<int>());
    };

    Value max_score = 0;

    for (;;) {
        distribution.front() = 0;
        Value sum = std::accumulate(distribution.begin(), distribution.end(), 0);
        Value max = 100;
        if (sum == max) {
            max = *std::max_element(distribution.begin(), distribution.end());
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

        Value rest = 100 - std::accumulate(distribution.begin(), distribution.end(), 0);
        distribution.front() = rest;

        max_score = std::max(max_score, apply());

        if (distribution.front() == 100) {
            break;
        }
    }

    fmt::print("1: {}\n", max_score);
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

    part1(ingredients);
}
